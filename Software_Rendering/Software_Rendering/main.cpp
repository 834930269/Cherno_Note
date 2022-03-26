#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "our_gl.h"

Model* model = NULL;
const int width = 800;
const int height = 800;

Vec3f light_dir(1, 1, 1);
Vec3f eye(0, -1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

struct GouraudShader : public IShader {
	Vec3f varying_intensity;	//通过顶点shader写入,由片元着色器读取
	mat<2, 3, float> varying_uv;  // same as above
	//模型的第i个三角形,第n个顶点
	virtual Vec4f vertex(int iface, int nthvert) {
		//设置设置三个点的uv
		varying_uv.set_col(nthvert, model->uv(iface, nthvert));
		Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));	//顶点
		gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;//顶点转到屏幕空间
		varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert) * light_dir);
		return gl_Vertex;
	}

	//bar是重心坐标,color是颜色
	virtual bool fragment(Vec3f bar, TGAColor& color) {
		float intensity = varying_intensity * bar;	//得到每个片元的
		Vec2f uv = varying_uv * bar;	//计算每个片元的uv
		color = model->diffuse(uv) * intensity;
		return false;	//不剪切
	}
};

int main(int argc,char **argv) {
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}
	//预计算MVP矩阵
	lookat(eye, center, up);
	viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	projection(-1.f / (eye - center).norm());
	light_dir.normalize();

	TGAImage image(width, height, TGAImage::RGB);
	TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

	GouraudShader shader;
	for (int i = 0; i < model->nfaces(); i++) {
		Vec4f screen_coords[3];
		for (int j = 0; j < 3; j++) {
			screen_coords[j] = shader.vertex(i, j);
		}
		triangle(screen_coords, shader, image, zbuffer);
	}

	image.flip_vertically(); // to place the origin in the bottom left corner of the image
	zbuffer.flip_vertically();
	image.write_tga_file("output.tga");
	zbuffer.write_tga_file("zbuffer.tga");

	delete model;

	return 0;
}
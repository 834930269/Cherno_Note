#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 0);
Model* model = NULL;
TGAImage *img = NULL;
const int width = 800;
const int height = 800;

void line(Vec2i x0, Vec2i x1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0.x - x1.x) < std::abs(x0.y - x1.y)) { // if the line is steep, we transpose the image 
        std::swap(x0.x, x0.y);
        std::swap(x1.x, x1.y);
        steep = true;
    }
    if (x0.x > x1.x) { // make it left−to−right 
        std::swap(x0.x, x1.x);
        std::swap(x0.y, x1.y);
    }

    int dx = x1.x - x0.x;
    int dx2 = dx * 2;
    int dy = x1.y - x0.y;
    int derror = std::abs(dy)*2;//每次增加多少
    int error = 0;    //累计步进
    int y = x0.y;         //初始步进
    for (int x = x0.x; x <= x1.x; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error += derror;    //累加步进
        if (error > dx) {   //如果步进大于.5,更新y
            y += (x1.y > x0.y ? 1 : -1);
            error -= dx2;
        }
    }
}

//求重心坐标,目的是为了求Z-buffer
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i = 2;i--;) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }

    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2) {
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }
    return Vec3f(-1, 1, 1);
}

void triangle(Vec3f *pts,float *zbuffer, TGAImage& image, TGAColor color) {
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    //算出三角形的边界范围
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j]=std::max(0.f,std::min(bboxmin[j],pts[i][j]));
            bboxmax[j]=std::min(clamp[j],std::max(bboxmax[j],pts[i][j]));
        }
    }
    Vec3f P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            //求重心坐标
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2],P);
            //如果不在三角形内,就继续
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            P.z = 0;
            //根据重心坐标求当前像素的z坐标,bc_screen是三个顶点的权重
            //把每个顶点上的z坐标按这个权重加起来就是z坐标
            for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
            //如果原来的z坐标小于新的z坐标,则更新
            if (zbuffer[int(P.x + P.y * width)] < P.z) {
                zbuffer[int(P.x + P.y * width)] = P.z;
                //如果比缓存的z坐标要小,，就不必在画了
                image.set(P.x, P.y, color);
            }
        }
    }

}

Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
        img = new TGAImage(width, height, TGAImage::RGB);
    }
    img->read_tga_file("obj/african_head_diffuse.tga");

    float* zbuffer = new float[width * height];
    //初始化为最小值
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec3f pts[3];
        for (int i = 0; i < 3; i++)pts[i] = world2screen(model->vert(face[i]));
        triangle(pts, zbuffer, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }

    image.flip_vertically();	//希望原点在左下方
	image.write_tga_file("output.tga");

    //释放model
    delete model;
	return 0;
}
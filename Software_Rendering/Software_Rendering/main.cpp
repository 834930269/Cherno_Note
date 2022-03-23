#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 0);
Model* model = NULL;
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
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    if (t0.y == t1.y && t0.y == t2.y) return; // i dont care about degenerate triangles
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
	
    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model->nfaces(); ++i) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; ++j) {
            Vec3f v = model->vert(face[j]);
            //计算屏幕坐标(世界坐标转换为屏幕坐标)
            screen_coords[j] = Vec2i((v.x + 1.) * width / 2, (v.y + 1.) * height / 2);
            //计算世界坐标
            world_coords[j] = v;    
        }
        //计算法线,这个是叉积,通过重载^方式做的
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        //计算光照强度,点积
        float intensity = n * light_dir;
        if (intensity > 0) {
            //绘制,做了一次背面剔除,如果光照强度小于0,代表不受光
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    image.flip_vertically();	//希望原点在左下方
	image.write_tga_file("output.tga");

    //释放model
    delete model;
	return 0;
}
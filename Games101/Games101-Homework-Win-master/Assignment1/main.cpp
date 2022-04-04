#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

/// <summary>
/// ��ͼ�任,ʵ�����ǽ�����ת��������ռ�(λ��??)
/// ֻ��Ҫƽ��
/// 
/// ����ģ�͸㶨��ת����,view�㶨�������(ʱ����ԭ��)����ϵ
/// ���Ծ�����view��MOdel�ϲ���MV����
/// </summary>
/// <param name="eye_pos"></param>
/// <returns></returns>
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

//ģ�͵ı任����,���ǰ���ת,���Ÿ㶨
Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float rag = rotation_angle * MY_PI / 180.0;
    Eigen::Matrix4f RotateZ,RotateX,RotateY;
    //ģ�͵ı任����

    //������ת����(��Z���Լ����)
    RotateZ << cos(rag), -sin(rag), 0, 0,
        sin(rag), cos(rag), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
    RotateX << 1, 0, 0, 0,
        0,cos(rag), sin(rag), 0,
        0,-sin(rag), cos(rag), 0,
        0, 0, 0, 1;
    RotateY << cos(rag),0 , -sin(rag), 0,
        0, 1, 0, 0,
        sin(rag), 0, cos(rag), 0,
        0, 0, 0, 1;
    return RotateX*RotateY* RotateZ*model;
}
/// <summary>
/// projection,��ƽ����ת����[-1,1]^3������������
/// </summary>
/// <param name="eye_fov"></param>
/// <param name="aspect_ratio">������</param>
/// <param name="zNear"></param>
/// <param name="zFar"></param>
/// <returns></returns>
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();


    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    
    Eigen::Matrix4f persp_to_ortho = Eigen::Matrix4f::Identity();//͸��->�����ġ���ѹ������
    persp_to_ortho << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;
    // 
    //�ӽ�ƽ������ͶӰ����Ļ,�����ƽ��
    //����ͶӰ������Ҫת��[-1,1]����������
    //MVP֮�����NDC�ռ�
    float half_eye_fovY = eye_fov / 2 / 180.0 * MY_PI;
    //�ȼ����ƽ���4�����ֵ
    float top = zNear * tan(half_eye_fovY);
    float bottom = -top;
    float right = aspect_ratio * top;//aspect_ratio�ǳ�������
    float left = -right;//׶���Ѿ�����˷��飨�����������嶼�п��ܣ�

    //�����任1.��ƽ�������ƶ���ԭ��
    Eigen::Matrix4f ortho_translate = Eigen::Matrix4f::Identity();//�����������Ƶ�ԭ��
    ortho_translate << 1, 0, 0, -(right + left) / 2,
        0, 1, 0, -(top + bottom) / 2,
        0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0, 1;

    Eigen::Matrix4f ortho_scale = Eigen::Matrix4f::Identity();//�����鳤����ȫ���2
    ortho_scale << 2 / (right - left), 0, 0, 0,
        0, 2 / (top - bottom), 0, 0,
        0, 0, 2 / (zNear - zFar), 0,
        0, 0, 0, 1;

    projection = ortho_scale * ortho_translate * persp_to_ortho;
    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 45;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
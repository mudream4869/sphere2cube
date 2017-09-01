#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>

#include "sphere2cube.h"

using std::tie;

typedef std::pair<float, float> vec2f;
typedef std::vector<float> VF;
typedef std::vector<VF> VVF;

const float pi = M_PI; 
const float doub_pi = pi*2; 
const float half_pi = pi/2.0;
const float inv_pi = 1/pi;

Sphere2Cube::Sphere2Cube(int TILESIZE){

    tile_size = TILESIZE;
    half_size = (TILESIZE - 1.0) / 2;
    inv_half_size = 1/half_size;

    puts("Sphere2Cube : Perform cache angles...");

    cache_zp = VVF(TILESIZE, VF(TILESIZE));
    cache_zm = VVF(TILESIZE, VF(TILESIZE));
    cache_xypm = VVF(TILESIZE, VF(TILESIZE));
    cache_phi = VVF(TILESIZE, VF(TILESIZE));

    for(int tile_y = 0; tile_y < tile_size; tile_y++){
        float y = tile_y*inv_half_size - 1;
        for(int tile_x = 0; tile_x < tile_size; tile_x++){
            float x = tile_x*inv_half_size - 1;
            float inv_r = 1/sqrt(x*x + y*y + 1);
            cache_zp[tile_y][tile_x] = acos(inv_r);
            cache_zm[tile_y][tile_x] = acos(-inv_r);
            cache_xypm[tile_y][tile_x] = acos(y*inv_r);
            if(x != 0)
                cache_phi[tile_y][tile_x] = atan(y/x);
        }
    }

    puts("Sphere2Cube : Perform cache angles ok.");

    face_func[0] = &Sphere2Cube::func_up;
    face_func[1] = &Sphere2Cube::func_front;
    face_func[2] = &Sphere2Cube::func_right;
    face_func[3] = &Sphere2Cube::func_back;
    face_func[4] = &Sphere2Cube::func_left;
    face_func[5] = &Sphere2Cube::func_down;

    return;
}

void Sphere2Cube::transform(const cv::Mat& sphere_image, Faces& ret){

    std::thread prc[6];

    for(int lx = 0;lx < 6;lx++){
        prc[lx] = std::thread([this, lx, sphere_image, &ret](){
            int sphere_height = sphere_image.rows, sphere_width = sphere_image.cols;
            ret.faces[lx].create(tile_size, tile_size, CV_8UC3);
            for(int tile_y = 0; tile_y < tile_size; tile_y++){
                for(int tile_x = 0; tile_x < tile_size; tile_x++){
                    float theta, phi;
                    tie(theta, phi) = (this->face_func[lx])(*this, tile_y, tile_x);
                    int sp_x = this->phi2width(sphere_width, phi);
                    int sp_y = this->theta2height(sphere_height, theta);
                    ret.faces[lx].at<cv::Vec3b>(tile_y, tile_x) = sphere_image.at<cv::Vec3b>(sp_y, sp_x);
                }
            }
        });
    }

    for(int lx = 0;lx < 6;lx++)
        prc[lx].join();

    return;
}

float Sphere2Cube::update_phi(float phi, int major_dir, int minor_dir, float major_m, float major_p, float minor_m, float minor_p) const{
    if(major_dir < half_size)
        return phi + major_m;
    else if(major_dir > half_size)
        return phi + major_p;
    else if(minor_dir < half_size)
        return minor_m;
    else
        return minor_p;
}

vec2f Sphere2Cube::func_up(int tile_y, int tile_x){
    float theta = cache_zp[tile_y][tile_x];
    float phi = cache_phi[tile_x][tile_y];
    phi = update_phi(phi, tile_y, tile_x, pi, 0, -half_pi, half_pi);
    return vec2f(theta, phi);
}

vec2f Sphere2Cube::func_front(int tile_y, int tile_x){
    float theta = cache_xypm[tile_size - tile_y - 1][tile_size - tile_x - 1];
    float phi = cache_phi[tile_x][tile_size - 1];
    phi = update_phi(phi, tile_y, tile_x, 0, 0, -half_pi, half_pi);
    return vec2f(theta, phi);
}

vec2f Sphere2Cube::func_right(int tile_y, int tile_x){
    float theta, phi;
    tie(theta, phi) = func_front(tile_y, tile_x);
    phi += half_pi;
    if(phi > doub_pi) phi -= doub_pi;
    return vec2f(theta, phi);
}

vec2f Sphere2Cube::func_back(int tile_y, int tile_x){
    float theta, phi;
    tie(theta, phi) = func_front(tile_y, tile_x);
    phi += 2*half_pi;
    if(phi > doub_pi) phi -= doub_pi;
    return vec2f(theta, phi);
}

vec2f Sphere2Cube::func_left(int tile_y, int tile_x){
    float theta, phi;
    tie(theta, phi) = func_front(tile_y, tile_x);
    phi += 3*half_pi;
    if(phi > doub_pi) phi -= doub_pi;
    return vec2f(theta, phi);
}

vec2f Sphere2Cube::func_down(int tile_y, int tile_x){
    float theta = cache_zm[tile_y][tile_x];
    float phi = cache_phi[tile_x][tile_size - tile_y - 1];
    phi = update_phi(phi, tile_y, tile_x, 0, pi, -half_pi, half_pi);
    return vec2f(theta, phi);
}

float Sphere2Cube::phi2width(int width, float phi) const{
    float x = 0.5 * width*(phi*inv_pi + 1);
    if(x < 1)
        return x + width;
    else if(x > width)
        return x - width;
    else
        return x;
}

float Sphere2Cube::theta2height(int height, float theta) const{
    return height * theta * inv_pi;
}

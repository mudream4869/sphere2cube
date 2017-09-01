#ifndef SPHERE2CUBE_HEADER
#define SPHERE2CUBE_HEADER

#include <vector>
#include <utility>
#include <functional>

#include <opencv2/core/core.hpp>

struct Faces{
    cv::Mat faces[6]; 
}; 

class Sphere2Cube{
public:
    Sphere2Cube(int TILESIZE);

    void transform(const cv::Mat& sphere_image, Faces& ret);

private:
    typedef std::pair<float, float> vec2f;
    typedef std::vector<float> VF;
    typedef std::vector<VF> VVF;

    int tile_size;
    float half_size, inv_half_size;

    VVF cache_zp, cache_zm, cache_xypm, cache_phi;

    std::function<vec2f(Sphere2Cube&, int,int)> face_func[6];

    float update_phi(float phi, int major_dir, int minor_dir, float major_m, float major_p, float minor_m, float minor_p) const;

    vec2f func_up(int tile_y, int tile_x);
    vec2f func_front(int tile_y, int tile_x);
    vec2f func_right(int tile_y, int tile_x);
    vec2f func_back(int tile_y, int tile_x);
    vec2f func_left(int tile_y, int tile_x);
    vec2f func_down(int tile_y, int tile_x);

    float phi2width(int width, float phi) const;
    float theta2height(int height, float theta) const;
};

#endif

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include <mukyu/sphere2cube/image.hpp>


namespace mukyu {
namespace sphere2cube {


struct Faces {
    Image faces[6];
};


class Sphere2Cube {
public:
    Sphere2Cube(int tileSize);

    void transform(const Image& img, Faces& ret);

private:
    typedef std::pair<float, float> Vec2f;

    int tileSize_;
    float halfSize_;
    float invHalfSize_;

    std::vector<std::vector<float>> cacheZp_;
    std::vector<std::vector<float>> cacheZm_;
    std::vector<std::vector<float>> cacheXypm_;
    std::vector<std::vector<float>> cachePhi_;

    Vec2f funcUp(int tileY, int tileX);
    Vec2f funcFront(int tileY, int tileX);
    Vec2f funcRight(int tileY, int tileX);
    Vec2f funcBack(int tileY, int tileX);
    Vec2f funcLeft(int tileY, int tileX);
    Vec2f funcDown(int tileY, int tileX);
};


} // namespace sphere2cube
} // namespace mukyu

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

    void transform(const Image& sphereImage, Faces& ret);

private:
    typedef std::pair<float, float> vec2f;
    typedef std::vector<float> VF;
    typedef std::vector<VF> VVF;

    int tileSize_;
    float halfSize, invHalfSize;

    VVF cacheZp, cacheZm, cacheXypm, cachePhi;

    std::function<vec2f(Sphere2Cube&, int, int)> faceFunc[6];

    float updatePhi(float phi,
                     int majorDir, int minorDir,
                     float majorM, float majorP,
                     float minorM, float minorP) const;

    vec2f funcUp(int tile_y, int tile_x);
    vec2f funcFront(int tile_y, int tile_x);
    vec2f funcRight(int tile_y, int tile_x);
    vec2f funcBack(int tile_y, int tile_x);
    vec2f funcLeft(int tile_y, int tile_x);
    vec2f funcDown(int tile_y, int tile_x);
};


} // namespace sphere2cube
} // namespace mukyu

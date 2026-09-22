#include <mukyu/sphere2cube/sphere2cube.hpp>

#include <exception>
#include <iostream>
#include <thread>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>


namespace {


const float pi = M_PI;
const float doubPI = pi * 2;
const float halfPI = pi / 2.0;
const float invPI = 1 / pi;


static int clampIndex(float v, int size) {
    int i = static_cast<int>(v);
    if (i < 0) {
        return 0;
    }
    if (i >= size) {
        return size - 1;
    }
    return i;
}

float phi2Width(int width, float phi) {
    float x = 0.5 * width * (phi * invPI + 1);
    if (x < 1) {
        return x + width;
    } else if (x > width) {
        return x - width;
    } else {
        return x;
    }
}

float theta2Height(int height, float theta) {
    return height * theta * invPI;
}

float updatePhi(float phi, float halfSize,
                int majorDir, int minorDir,
                float majorM, float majorP,
                float minorM, float minorP) {
    if (majorDir < halfSize) {
        return phi + majorM;
    } else if (majorDir > halfSize) {
        return phi + majorP;
    } else if (minorDir < halfSize) {
        return minorM;
    } else {
        return minorP;
    }
}


}


namespace mukyu {
namespace sphere2cube {


Sphere2Cube::Sphere2Cube(int tileSize) {
    tileSize_ = tileSize;
    halfSize_ = (tileSize - 1.0) / 2;
    invHalfSize_ = 1 / halfSize_;

    cacheZp_ = std::vector<std::vector<float>>(
        tileSize, std::vector<float>(tileSize));
    cacheZm_ = cacheZp_;
    cacheXypm_ = cacheZp_;
    cachePhi_ = cacheZp_;

    for (int tileY = 0; tileY < tileSize; tileY++) {
        float y = tileY * invHalfSize_ - 1;
        for (int tileX = 0; tileX < tileSize; tileX++) {
            float x = tileX * invHalfSize_ - 1;
            float invR = 1 / sqrt(x * x + y * y + 1);
            cacheZp_[tileY][tileX] = acos(invR);
            cacheZm_[tileY][tileX] = acos(-invR);
            cacheXypm_[tileY][tileX] = acos(y * invR);
            if (x != 0) {
                cachePhi_[tileY][tileX] = atan(y / x);
            }
        }
    }
}

void Sphere2Cube::transform(const Image& img, Faces& ret) {
    std::thread prc[6];
    std::exception_ptr errs[6];

    struct Joiner {
        std::thread* prc;
        ~Joiner() {
            for (int lx = 0; lx < 6; lx++) {
                if (prc[lx].joinable()) {
                    prc[lx].join();
                }
            }
        }
    } joiner{prc};

    std::vector<std::function<Sphere2Cube::Vec2f(Sphere2Cube &, int, int)>>
        faceFuncs {
            &Sphere2Cube::funcUp,
            &Sphere2Cube::funcFront,
            &Sphere2Cube::funcRight,
            &Sphere2Cube::funcBack,
            &Sphere2Cube::funcLeft,
            &Sphere2Cube::funcDown
        };

    for (int lx = 0; lx < 6; lx++) {
        prc[lx] = std::thread([this, lx, &img, &ret, &errs, &faceFuncs]() {
            try {
                int height = img.height;
                int width = img.width;
                ret.faces[lx].create(tileSize_, tileSize_);
                for (int tileY = 0; tileY < tileSize_; ++tileY) {
                    for (int tileX = 0; tileX < tileSize_; ++tileX) {
                        auto [theta, phi] = (faceFuncs[lx])(*this, tileY, tileX);
                        int x = clampIndex(phi2Width(width, phi), width);
                        int y = clampIndex(theta2Height(height, theta), height);
                        memcpy(ret.faces[lx].at(tileY, tileX),
                               img.at(y, x), Image::channels);
                    }
                }
            } catch (...) {
                errs[lx] = std::current_exception();
            }
        });
    }

    for (int lx = 0; lx < 6; lx++) {
        prc[lx].join();
    }

    for (int lx = 0; lx < 6; lx++) {
        if (errs[lx]) {
            std::rethrow_exception(errs[lx]);
        }
    }
}

Sphere2Cube::Vec2f Sphere2Cube::funcUp(int tileY, int tileX) {
    float theta = cacheZp_[tileY][tileX];
    float phi = cachePhi_[tileX][tileY];
    phi = updatePhi(phi, halfSize_, tileY, tileX, pi, 0, -halfPI, halfPI);
    return Vec2f(theta, phi);
}

Sphere2Cube::Vec2f Sphere2Cube::funcFront(int tileY, int tileX) {
    float theta = cacheXypm_[tileSize_ - tileY - 1][tileSize_ - tileX - 1];
    float phi = cachePhi_[tileX][tileSize_ - 1];
    phi = updatePhi(phi, halfSize_, tileY, tileX, 0, 0, -halfPI, halfPI);
    return Vec2f(theta, phi);
}

Sphere2Cube::Vec2f Sphere2Cube::funcRight(int tileY, int tileX) {
    auto [theta, phi] = funcFront(tileY, tileX);
    phi += halfPI;
    if (phi > doubPI) {
        phi -= doubPI;
    }
    return Vec2f(theta, phi);
}

Sphere2Cube::Vec2f Sphere2Cube::funcBack(int tileY, int tileX) {
    auto [theta, phi] = funcFront(tileY, tileX);
    phi += 2 * halfPI;
    if (phi > doubPI) {
        phi -= doubPI;
    }
    return Vec2f(theta, phi);
}

Sphere2Cube::Vec2f Sphere2Cube::funcLeft(int tileY, int tileX) {
    auto [theta, phi] = funcFront(tileY, tileX);
    phi += 3 * halfPI;
    if (phi > doubPI) {
        phi -= doubPI;
    }
    return Vec2f(theta, phi);
}

Sphere2Cube::Vec2f Sphere2Cube::funcDown(int tileY, int tileX) {
    float theta = cacheZm_[tileY][tileX];
    float phi = cachePhi_[tileX][tileSize_ - tileY - 1];
    phi = updatePhi(phi, halfSize_, tileY, tileX, 0, pi, -halfPI, halfPI);
    return Vec2f(theta, phi);
}



} // namespace sphere2cube
} // namespace mukyu

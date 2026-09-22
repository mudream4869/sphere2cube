#pragma once

#include <vector>
#include <cstddef>


namespace mukyu {
namespace sphere2cube {


// 8-bit RGB image, loaded/stored with stb_image / stb_image_write.
struct Image {
    static const int channels = 3;

    int width = 0;
    int height = 0;
    std::vector<unsigned char> data;

    void create(int w, int h);

    bool load(const char* filename);
    bool saveJPG(const char* filename, int quality = 90) const;

    unsigned char* at(int y, int x) {
        return &data[(static_cast<size_t>(y) * width + x) * channels];
    }

    const unsigned char* at(int y, int x) const {
        return &data[(static_cast<size_t>(y) * width + x) * channels];
    }
};


} // namespace sphere2cube
} // namespace mukyu

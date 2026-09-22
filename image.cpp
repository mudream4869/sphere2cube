#include "image.h"

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Image::create(int w, int h) {
    std::vector<unsigned char> buf(static_cast<size_t>(w)*h*channels, 0);
    data.swap(buf);
    width = w;
    height = h;
}

bool Image::load(const char* filename) {
    int w, h, comp;
    // Force 3 channels, so gray/alpha inputs are handled too.
    std::unique_ptr<unsigned char, void(*)(void*)> pixels(
        stbi_load(filename, &w, &h, &comp, channels), stbi_image_free);
    if (pixels == nullptr) {
        return false;
    }

    std::vector<unsigned char> buf(pixels.get(),
                                   pixels.get() + static_cast<size_t>(w)*h*channels);
    data.swap(buf);
    width = w;
    height = h;
    return true;
}

bool Image::save_jpg(const char* filename, int quality) const {
    if (data.empty()) {
        return false;
    }
    return stbi_write_jpg(filename, width, height, channels, data.data(), quality) != 0;
}

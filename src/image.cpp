#include <mukyu/sphere2cube/image.hpp>

#include <memory>
#include <vector>

#include <stb_image.h>
#include <stb_image_write.h>


namespace mukyu {
namespace sphere2cube {


void Image::create(int w, int h) {
    std::vector<unsigned char> buf(static_cast<size_t>(w)*h*channels, 0);
    data.swap(buf);
    width = w;
    height = h;
}

bool Image::load(const std::string& filename) {
    int w, h, comp;

    std::unique_ptr<unsigned char, void (*)(void *)> pixels(
        stbi_load(filename.c_str(), &w, &h, &comp, channels), stbi_image_free);
    if (pixels == nullptr) {
        return false;
    }

    std::vector<unsigned char> buf(
        pixels.get(),
        pixels.get() + static_cast<size_t>(w) * h * channels);

    data.swap(buf);
    width = w;
    height = h;
    return true;
}

bool Image::saveJPG(const std::string& filename, int quality) const {
    if (data.empty()) {
        return false;
    }

    return stbi_write_jpg(
        filename.c_str(), width, height, channels, data.data(), quality) != 0;
}


} // namespace sphere2cube
} // namespace mukyu

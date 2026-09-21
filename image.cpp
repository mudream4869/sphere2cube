#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Image::create(int w, int h){
    width = w;
    height = h;
    data.assign(static_cast<size_t>(w)*h*channels, 0);
}

bool Image::load(const char* filename){
    int w, h, comp;
    // Force 3 channels, so gray/alpha inputs are handled too.
    unsigned char* pixels = stbi_load(filename, &w, &h, &comp, channels);
    if(pixels == nullptr)
        return false;

    width = w;
    height = h;
    data.assign(pixels, pixels + static_cast<size_t>(w)*h*channels);
    stbi_image_free(pixels);
    return true;
}

bool Image::save_jpg(const char* filename, int quality) const{
    if(data.empty())
        return false;
    return stbi_write_jpg(filename, width, height, channels, data.data(), quality) != 0;
}

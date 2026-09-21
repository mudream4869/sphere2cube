#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include <cstddef>
#include <vector>

// 8-bit RGB image, loaded/stored with stb_image / stb_image_write.
struct Image{
    static const int channels = 3;

    int width = 0, height = 0;
    std::vector<unsigned char> data;

    void create(int w, int h);

    bool load(const char* filename);
    bool save_jpg(const char* filename, int quality = 90) const;

    unsigned char* at(int y, int x){
        return &data[(static_cast<size_t>(y)*width + x)*channels];
    }

    const unsigned char* at(int y, int x) const{
        return &data[(static_cast<size_t>(y)*width + x)*channels];
    }
};

#endif

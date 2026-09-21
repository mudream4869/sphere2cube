#include <chrono>
#include <cstdio>

#include "image.h"
#include "sphere2cube.h"

int main(int argc, char** argv){
    if(argc < 2){
        printf("Usage: %s [Panorama Filename]\n", argv[0]);
        return 1;
    }

    Image image;
    if(!image.load(argv[1])){
        printf("Failed to load image: %s\n", argv[1]);
        return 1;
    }

    Sphere2Cube s2c(540);
    Faces cube;

    auto t1 = std::chrono::steady_clock::now();
    s2c.transform(image, cube);
    auto t2 = std::chrono::steady_clock::now();

    printf("Cost %f s.\n", std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count());

    const char* names[6] = {"up.jpg", "front.jpg", "right.jpg", "back.jpg", "left.jpg", "down.jpg"};
    for(int lx = 0; lx < 6; lx++){
        if(!cube.faces[lx].save_jpg(names[lx])){
            printf("Failed to write image: %s\n", names[lx]);
            return 1;
        }
    }

    return 0;
}

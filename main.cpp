#include <chrono>
#include <exception>
#include <iostream>

#include <mukyu/sphere2cube/image.hpp>
#include <mukyu/sphere2cube/sphere2cube.hpp>


using mukyu::sphere2cube::Faces;
using mukyu::sphere2cube::Image;
using mukyu::sphere2cube::Sphere2Cube;


static int run(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [Panorama Filename]\n";
        return 1;
    }

    Image image;
    if (!image.load(argv[1])) {
        std::cerr << "Failed to load image: " << argv[1] << "\n";
        return 1;
    }

    Sphere2Cube s2c(540);
    Faces cube;

    auto t1 = std::chrono::steady_clock::now();
    s2c.transform(image, cube);
    auto t2 = std::chrono::steady_clock::now();

    std::cout << "Cost "
              << std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count()
              << " s.\n";

    const char* names[6] = {"up.jpg", "front.jpg", "right.jpg", "back.jpg", "left.jpg", "down.jpg"};
    for (int lx = 0; lx < 6; lx++) {
        if (!cube.faces[lx].save_jpg(names[lx])) {
            std::cerr << "Failed to write image: " << names[lx] << "\n";
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    try {
        return run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Error: unknown exception\n";
        return 1;
    }
}

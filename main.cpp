#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include "sphere2cube.h"

int main(int argc, char** argv){
    Sphere2Cube s2c(540);
    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Faces cube;

    auto t1 = std::chrono::steady_clock::now();
    s2c.transform(image, cube);
    auto t2 = std::chrono::steady_clock::now();

    printf("Cost %f s.\n", std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count());

    cv::imwrite("up.jpg", cube.faces[0]);
    cv::imwrite("front.jpg", cube.faces[1]);
    cv::imwrite("right.jpg", cube.faces[2]);
    cv::imwrite("back.jpg", cube.faces[3]);
    cv::imwrite("left.jpg", cube.faces[4]);
    cv::imwrite("down.jpg", cube.faces[5]);

    return 0;
}

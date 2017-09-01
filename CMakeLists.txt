cmake_minimum_required(VERSION 2.8)

project( Sphere2Cube )

set (CMAKE_CXX_STANDARD 11)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( OpenCV 2 REQUIRED )
add_executable( main main.cpp sphere2cube.cpp )
target_link_libraries( main ${OpenCV_LIBS} )

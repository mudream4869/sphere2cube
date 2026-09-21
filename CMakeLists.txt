cmake_minimum_required(VERSION 3.5)

project( Sphere2Cube )

set (CMAKE_CXX_STANDARD 11)

find_package( Threads REQUIRED )

add_executable( main main.cpp sphere2cube.cpp image.cpp )
target_include_directories( main PRIVATE third_party/stb )
target_link_libraries( main Threads::Threads )

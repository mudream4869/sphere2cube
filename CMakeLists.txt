cmake_minimum_required(VERSION 3.20)
project(sphere2cube LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(Threads REQUIRED)

add_library(sphere2cube
    src/image.cpp
    src/sphere2cube.cpp
)
target_include_directories(sphere2cube
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
)
target_compile_features(sphere2cube PUBLIC cxx_std_23)
target_link_libraries(sphere2cube PUBLIC Threads::Threads)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE sphere2cube)

# sphere2cube

Tools to convert equirectangular projection 360 panoramas in 6 faces of the cube.

Migrate from a [python version](https://github.com/flash286/sphere2cube).

## Requirement

* A C++11 compiler
* CMake 3.10+

Image IO uses [stb_image / stb_image_write](https://github.com/nothings/stb),
vendored in `third_party/stb`, so there is no external library to install.

## Compile and Run

### Compile

```
cmake -S . -B build
cmake --build build
```

### Run

```
./build/main [Panorama Filename]
```

For example, `./build/main sample/sample.png` writes the six faces
(`front.jpg`, `back.jpg`, ...) to the current directory.

## Usage

See `main.cpp`


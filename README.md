# sphere2cube

Tools to convert equirectangular projection 360 panoramas in 6 faces of the cube.

Migrate from a [python version](https://github.com/flash286/sphere2cube).

## Requirement

* cmake
* A C++11 compiler

Image IO uses [stb_image / stb_image_write](https://github.com/nothings/stb),
vendored in `third_party/stb`, so there is no external library to install.

## Compiler and Run

### Compile

```
mkdir build
cd build
cmake ..
make
```

### Run

```
./main [Panorama Filename]
```

## Usage

See `main.cpp`

## TODO

* Remove cmake

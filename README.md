# sphere2cube

Tools to convert equirectangular projection 360 panoramas in 6 faces of the cube.

Migrate from a [python version](https://github.com/flash286/sphere2cube).

## Requirement

* A C++11 compiler

Image IO uses [stb_image / stb_image_write](https://github.com/nothings/stb),
vendored in `third_party/stb`, so there is no external library to install.

## Compiler and Run

### Compile

```
make
```

Or without `make`:

```
g++ -O2 -std=c++11 -pthread -Ithird_party/stb *.cpp -o main
```

### Run

```
./main [Panorama Filename]
```

## Usage

See `main.cpp`


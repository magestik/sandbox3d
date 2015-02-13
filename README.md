sandbox3d
=========

### Introduction

A sandbox that I use to test some graphic techniques.

### Dependencies

Libraries needed :
- Qt 5
- OpenGL 3.3
- Assimp

### TODO

- Fog
- Ambient Occlusion
- Depth of Field

Implement a way to select objects (pick buffer ?).

### Build

Compile C++ code, and preprocess shaders:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ..
$ ./scripts/deploy_shaders
```

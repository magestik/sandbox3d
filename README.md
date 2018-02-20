sandbox3d
=========

### Introduction

A sandbox that I use to test some graphic techniques.

Implemented :
- Light Prepass,
- Simple Fog,
- Normal mapping,
- Shadow mapping (PCF),
- Tone Mapping (the scene is first rendered in HDR format),
- Bloom,
- FXAA (just plugged Timothy Lottes implementation),
- Cel Shading,
- SSAO.

### Build

Compile C++ code, and preprocess shaders :
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

### Dependencies

This project is designed to run on GNU/Linux and does not support Windows (maybe using GLEW would be enough to make it work).

Libraries needed :
- OpenGL 3.3 / Vulkan 1.0.4
- TinyXML2

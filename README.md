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
- Pick Buffer.

TODO :
- Ray of God,
- Ambient Occlusion,
- Depth of Field,
- PBR.

### Build

Compile C++ code, and preprocess shaders :
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Dependencies

This project is designed to run on GNU/Linux and does not support Windows (maybe using GLEW would be enough to make it work).

Libraries needed :
- Qt 5
- OpenGL 3.3
- Assimp
- TinyXML2
- cpp (to preprocess shaders)

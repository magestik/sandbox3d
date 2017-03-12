sandbox3d
=========

[![Build Status](https://travis-ci.org/magestik/sandbox3d.svg?branch=master)](https://travis-ci.org/magestik/sandbox3d)

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
- Pick Buffer.

TODO :
- Ray of God,
- Ambient Occlusion,
- Depth of Field,
- PBR,
- Cel Shading,
- VXGI (https://developer.nvidia.com/vxgi),
- Visibility buffer (http://www.confettispecialfx.com/gdce-2016-the-filtered-and-culled-visibility-buffer-2/),
- Hierarchical Z-buffer (http://www.nickdarnell.com/hierarchical-z-buffer-occlusion-culling/)
- Raytraced shadow maps,
- Cascaded shadow maps,
- AXAA (http://dl.acm.org/citation.cfm?id=2945129).


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
- Assimp
- TinyXML2
- jansson
- OpenGL 3.3 / Vulkan 1.0.4

Other requierements :
- glslc (to preprocess shaders)
- xmllint (to check render.xml)

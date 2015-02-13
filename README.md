sandbox3d
=========

### Introduction

A sandbox that I use to test some graphic techniques.

### Dependencies

Libraries needed :
- Qt 5
- OpenGL 3.3
- Assimp
- TinyXML2

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

### Render Control

Configured with Render.xml

#### Depth Control

By default, depth write and depth test is disabled.

<depth mask="GL_BOOLEAN" func="GL_ENUM" />

Enable depth test and compare using 'func' (see glDepthFunc). Depth write is enabled if 'mask' is not GL_FALSE (see glDepthMask).

#### Blend Control

By default, blending is disabled.

<blend sfactor="GL_ENUM" dfactor="GL_ENUM" equation="GL_DEPTH_FUNC" />

Enable blending and specify equation (see glBlendFunc) and factors (see glBlendEquation).

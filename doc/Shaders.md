# Shaders

## Intro

Shader sources are not written in pure GLSL. 

I wanted to have some include mechanisms so the shaders are preprocessed (using cpp).

## Problems

Some keywords are not supported by the C preprocessor and will provoke errors :
```
#version
#extension
```

The must be avoided ! Instead you must use :
```
#pragma GLSL(version) 
#pragma GLSL(extension)
```

They will be replaced by the correct keywords during preprocessing.

## Preprocessing

Shaders are preprocessed using this command (from project source directory) :
```
./scripts/deploy_shaders
```

Preprocessed files are placed in data/shaders/.


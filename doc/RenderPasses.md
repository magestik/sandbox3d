# Render Passes

Render passes are entirely configurable (with data/render.xml).

## Targets

Define all Render Targets.

## Pipelines

Define all Pipelines.

### Depth Control

By default, the depth functionalities are disabled (both zWrite and zRead). 

```
<depth mask="GL_BOOLEAN" func="GL_ENUM" />
```

This is used to enable depth test and set the compare function (see [glDepthFunc](https://www.opengl.org/sdk/docs/man/html/glDepthFunc.xhtml)).
It is also used to enable or disable zWrite (see [glDepthMask](https://www.opengl.org/sdk/docs/man/html/glDepthMask.xhtml)).

### Stencil Control

By default, the stencil functionalities are disabled (both write and compare). 

```
<stencil mask="GL_UNSIGNED" func="GL_ENUM" />
```

This is used to enable stencil test and set the compare function (see [glStencilFunc](https://www.opengl.org/sdk/docs/man/html/glStencilFunc.xhtml)).
It is also used to change to stencil mask (see [glStencilMask](https://www.opengl.org/sdk/docs/man/html/glStencilMask.xhtml)).

### Blend Control

By default, blending is disabled. To enable it use :

```
<blend sfactor="GL_ENUM" dfactor="GL_ENUM" equation="GL_ENUM" />
```

This is used to enable blending and specify equation (see [glBlendFunc](https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml)) and source/dest factors (see [glBlendEquation](https://www.opengl.org/sdk/docs/man/html/glBlendEquation.xhtml)).

### Samplers

```
<sampler name="string" min_filter="GL_ENUM" mag_filter="GL_ENUM" wrap_s="GL_ENUM" wrap_t="GL_ENUM" wrap_r="GL_ENUM" compare_mode="GL_ENUM" compare_func="GL_ENUM"/>
```

This is used to configure samplers (see [glSamplerParameter](https://www.opengl.org/sdk/docs/man/html/glSamplerParameter.xhtml)).

### Primitives Setup

TODO

[glCullFace](https://www.opengl.org/sdk/docs/man/html/glCullFace.xhtml)
[glPolygonMode](https://www.opengl.org/sdk/docs/man/html/glPolygonOffset.xhtml)
[glPolygonOffset](https://www.opengl.org/sdk/docs/man/html/glPolygonOffset.xhtml)

### Enable

By default every GL feature is disabled.

```
<enable cap="GL_ENUM" />
```

This enable a GL feature (see [glEnable](https://www.opengl.org/sdk/docs/man/html/glEnable.xhtml)).
WARNING: This should not be used directly for supported features (GL_DEPTH_TEST and GL_BLEND) !
This is just an helper for functionnality not yet supported in this file.

## Techniques

Define all Techniques.

### Technique

Define a Technique.

#### Pass

Define a Pass.


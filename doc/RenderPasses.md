# Render Passes

Render passes are entirely configurable (with data/render.xml).

## Targets

Define all Render Targets.

## Technique

### Depth Control

By default, the depth functionalities are disabled (both zWrite and zRead). 

```
<depth mask="GL_BOOLEAN" func="GL_ENUM" />
```

This is used to enable depth test and set the compare function (see [glDepthFunc](https://www.opengl.org/sdk/docs/man4/html/glDepthFunc.xhtml)).
It is also used to enable or disable zWrite (see [glDepthMask](https://www.opengl.org/sdk/docs/man4/html/glDepthMask.xhtml)).

### Blend Control

By default, blending is disabled. To enable it use :

```
<blend sfactor="GL_ENUM" dfactor="GL_ENUM" equation="GL_ENUM" />
```

This is used to enable blending and specify equation (see [glBlendFunc](https://www.opengl.org/sdk/docs/man4/html/glBlendFunc.xhtml)) and source/dest factors (see [glBlendEquation](https://www.opengl.org/sdk/docs/man4/html/glBlendEquation.xhtml)).

### Enable

By default every GL feature is disabled.

```
<enable cap="GL_ENUM" />
```

This enable a GL feature (see [glEnable](https://www.opengl.org/sdk/docs/man4/html/glEnable.xhtml)).
WARNING: This should not be used directly for supported features (GL_DEPTH_TEST and GL_BLEND) !
This is just an helper for functionnality not yet supported in this file.

### Pass

#### Sampler Control

```
<sampler name="string" min_filter="GL_ENUM" mag_filter="GL_ENUM" wrap_s="GL_ENUM" wrap_t="GL_ENUM" wrap_r="GL_ENUM" compare_mode="GL_ENUM" compare_func="GL_ENUM"/>
```

This is used to configure samplers (see [glSamplerParameter](https://www.opengl.org/sdk/docs/man/html/glSamplerParameter.xhtml)).

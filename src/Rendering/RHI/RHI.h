#pragma once

#include "config.h"

#if HAVE_OPENGL
#	define GL_GLEXT_PROTOTYPES
#	if HAVE_GL_GLCOREARB_H
#		include <GL/glcorearb.h>
#	elif HAVE_GL_GL3_H
#               include <GL/gl3.h>
#       elif HAVE_GL_GL_H
#               include <GL/gl.h>
#	elif HAVE_OPENGL_GL_H
#               include <OpenGL/gl.h>
#	endif
#endif // HAVE_OPENGL

#if HAVE_VULKAN
#	include <vulkan/vulkan.h>
#endif // HAVE_VULKAN

//
// math vectors
#include <Vector.h>

//
// Enumerations
#include "RHI_enums.h"

//
// Shader Module
#include "ShaderModule.h"

//
// Structurs
#include "RHI_structs.h"

//
// Render Pass
#include "Pass.h"

//
// Framebuffer
#include "Framebuffer.h"

//
// Sampler
#include "Sampler.h"

//
// Pipeline
#include "Pipeline.h"

//
// Command Buffer
#include "CommandBuffer.h"

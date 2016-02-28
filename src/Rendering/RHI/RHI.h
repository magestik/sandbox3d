#pragma once

#if HAVE_OPENGL
#	define GL_GLEXT_PROTOTYPES
#	include <GL/glcorearb.h>
#endif // HAVE_OPENGL

#if HAVE_VULKAN
#	include <vulkan/vulkan.h>
#endif // HAVE_VULKAN

//
// Framebuffer
#include "Framebuffer.h"

//
// Render Pass
#include "Pass.h"

//
// Command Buffer
#include "CommandBuffer.h"

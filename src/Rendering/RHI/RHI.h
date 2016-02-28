#pragma once

#if HAVE_OPENGL
#	define GL_GLEXT_PROTOTYPES
#	include <GL/glcorearb.h>
#endif // HAVE_OPENGL

#if HAVE_VULKAN
#	include <vulkan/vulkan.h>
#endif // HAVE_VULKAN

//
// Enumerations
#include "RHI_enums.h"

//
// Render Pass
#include "Pass.h"

//
// Framebuffer
#include "Framebuffer.h"

//
// Render Pass
#include "Pipeline.h"

//
// Command Buffer
#include "CommandBuffer.h"

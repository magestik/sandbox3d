#pragma once

#if HAVE_OPENGL
#	define GL_GLEXT_PROTOTYPES
#	include <GL/glcorearb.h>
#endif // HAVE_OPENGL

#if HAVE_VULKAN || 1
#	include <vulkan/vulkan.h>
#endif // HAVE_VULKAN

//
// math vectors
#include <Vector.h>

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
// Sampler
#include "Sampler.h"

//
// Render Pass
#include "Pipeline.h"

//
// Command Buffer
#include "CommandBuffer.h"

#pragma once

#include <vector>

#include <GPU.h>

#include <Vector.h>

#if HAVE_OPENGL
#	include "OpenGL/Subpass.h"
#endif // HAVE_OPENGL

class Rendering;

namespace RHI
{

class RenderPass
{
public:

	struct SubpassDescription
	{
		std::vector<int> aColorAttachments;
		int depthAttachment;
	};

	//
	// Constructors / Destructor
	RenderPass		(void);
	RenderPass		(const RenderPass & pass) = delete;
	RenderPass		(RenderPass && pass);
	RenderPass		(const std::vector<SubpassDescription> & aPass);
	~RenderPass		(void);

	//
	// Operators
	RenderPass &	operator =		(const RenderPass & pass) = delete;
	RenderPass &	operator =		(RenderPass && pass);

	//
	// Vulkan-like API
	bool	BeginRenderPass		(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent);
	bool	BeginRenderPass		(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color);
	bool	BeginRenderPass		(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil);
	void	EndRenderPass		(void);

	void	NextSubpass			(void);

protected:

#if HAVE_OPENGL
	std::vector<Subpass> m_aPass;
	int m_iCurrentSubpass;
	bool m_bActive; // just for debugging
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkRenderPass m_renderPass;
#endif // HAVE_VULKAN
};

}

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

	friend class CommandBuffer;

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

protected:

#if HAVE_OPENGL
	bool	Begin		(bool defaultFramebuffer);
	void	Next		(void);
	void	End			(void);

	void	SetCurrent	(unsigned int current);

	std::vector<Subpass> m_aPass;
	unsigned int m_uCurrentSubpass;
	bool m_bDefaultFramebuffer;
	bool m_bActive; // just for debugging
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkRenderPass m_renderPass;
#endif // HAVE_VULKAN
};

}

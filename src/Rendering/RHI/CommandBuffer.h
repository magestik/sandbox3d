#pragma once

namespace RHI
{

class CommandBuffer
{
public:

	//
	// Constructors / Destructor
	CommandBuffer		(void);
	~CommandBuffer		(void);

	//
	//
	bool	Begin		(void);
	bool	End			(void);

	//
	// Commands
	bool	BeginRenderPass		(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent);
	bool	BeginRenderPass		(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color);
	bool	BeginRenderPass		(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, float depth, unsigned int stencil);
	bool	BeginRenderPass		(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil);
	void	EndRenderPass		(void);

	void	NextSubpass			(void);

private:

	RenderPass * m_pCurrentPass;
	bool m_bActive;

};

}

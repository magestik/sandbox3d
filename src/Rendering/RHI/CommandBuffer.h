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
	// Render Pass
	bool	BeginRenderPass			(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent);
	bool	BeginRenderPass			(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color);
	bool	BeginRenderPass			(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, float depth, unsigned int stencil);
	bool	BeginRenderPass			(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil);

	void	NextSubpass				(void);

	void	EndRenderPass			(void);

	//
	// Resource Binding
	bool	Bind					(Pipeline & pipeline);

	//
	// Draw Commands
	bool	Draw					(unsigned int vertexCount, unsigned int firstVertex);
	bool	Draw					(unsigned int vertexCount, unsigned int firstVertex, unsigned int instanceCount);
	bool	Draw					(unsigned int vertexCount, unsigned int firstVertex, unsigned int instanceCount, unsigned int firstInstance);
	bool	DrawIndexed				(unsigned int indexCount, unsigned int firstVertex, unsigned int vertexOffset);
	bool	DrawIndexed				(unsigned int indexCount, unsigned int firstVertex, unsigned int vertexOffset, unsigned int instanceCount);
	bool	DrawIndexed				(unsigned int indexCount, unsigned int firstVertex, unsigned int vertexOffset, unsigned int instanceCount, unsigned int firstInstance);

public:

#if HAVE_OPENGL
	RenderPass * m_pCurrentPass;
	Pipeline * m_pCurrentPipeline;
	bool m_bActive;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkCommandBuffer m_commandBuffer;
#endif // HAVE_VULKAN
};

}

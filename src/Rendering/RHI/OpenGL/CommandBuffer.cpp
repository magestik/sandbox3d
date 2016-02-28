#include "../RHI.h"

/**
 * @brief Default constructor
 */
RHI::CommandBuffer::CommandBuffer(void)
: m_pCurrentPass(nullptr)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Destructor
 */
RHI::CommandBuffer::~CommandBuffer(void)
{
	assert(!m_bActive);
}

/**
 * @brief RHI::CommandBuffer::Begin
 * @return
 */
bool RHI::CommandBuffer::Begin(void)
{
	m_bActive = true;

	return(true);
}

/**
 * @brief RHI::CommandBuffer::End
 * @return
 */
bool RHI::CommandBuffer::End(void)
{
	m_bActive = false;

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param color
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param depth
 * @param stencil
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, float depth, unsigned int stencil)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glClearDepthf(depth);
	glClearStencil(stencil);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param color
 * @param depth
 * @param stencil
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glClearColor(color.x, color.y, color.z, color.w);
	glClearDepthf(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::EndRenderPass
 */
void RHI::CommandBuffer::EndRenderPass(void)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_pCurrentPass->End();
	m_pCurrentPass = nullptr;
}

/**
 * @brief RHI::CommandBuffer::NextSubpass
 */
void RHI::CommandBuffer::NextSubpass(void)
{
	assert(m_bActive);

	m_pCurrentPass->Next();

}

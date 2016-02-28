#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief RenderPass::RenderPass
 */
RHI::RenderPass::RenderPass(void)
: m_iCurrentSubpass(0)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Move constructor
 * @param pass
 */
RHI::RenderPass::RenderPass(RenderPass && pass)
: m_iCurrentSubpass(pass.m_iCurrentSubpass)
, m_bActive(pass.m_bActive)
{
	pass.m_iCurrentSubpass = 0;
	pass.m_bActive = false;
}

/**
 * @brief RenderPass::RenderPass
 * @param root
 * @param rendering
 */
RHI::RenderPass::RenderPass(const std::vector<SubpassDescription> & aPassDesc)
: m_iCurrentSubpass(0)
, m_bActive(false)
, m_aPass()
{
	for (const SubpassDescription & desc : aPassDesc)
	{
		m_aPass.push_back(Subpass(desc.aColorAttachments));
	}
}

/**
 * @brief Pass::~Pass
 */
RHI::RenderPass::~RenderPass(void)
{

}

/**
 * @brief Pass::operator =
 * @param pass
 * @return
 */
RHI::RenderPass & RHI::RenderPass::operator=(RenderPass && pass)
{
	m_iCurrentSubpass = pass.m_iCurrentSubpass;
	pass.m_iCurrentSubpass = 0;

	m_bActive = pass.m_bActive;
	pass.m_bActive = false;

	m_aPass = std::move(pass.m_aPass);

	return(*this);
}

/**
 * @brief Pass::BeginRenderPass
 * @param offset
 * @param extent
 * @return
 */
bool RHI::RenderPass::BeginRenderPass(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent)
{
	assert(!m_bActive);
	assert(m_iCurrentSubpass == 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	if (0 != fb.m_uFramebufferObject)
	{
		Subpass & subpass = m_aPass.front();

		unsigned int size = subpass.m_aDrawBuffers.size();

		if (size > 0)
		{
			glDrawBuffers(size, subpass.m_aDrawBuffers.data());
		}
	}

	glViewport(offset.x, offset.y, extent.x, extent.y);

	m_bActive = true;

	return(true);
}

/**
 * @brief Pass::BeginRenderPass
 * @param offset
 * @param extent
 * @param color
 * @return
 */
bool RHI::RenderPass::BeginRenderPass(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color)
{
	assert(!m_bActive);
	assert(m_iCurrentSubpass == 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	if (0 != fb.m_uFramebufferObject)
	{
		Subpass & subpass = m_aPass.front();

		unsigned int size = subpass.m_aDrawBuffers.size();

		if (size > 0)
		{
			glDrawBuffers(size, subpass.m_aDrawBuffers.data());
		}
	}

	glViewport(offset.x, offset.y, extent.x, extent.y);
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	m_bActive = true;

	return(true);
}

/**
 * @brief Pass::BeginRenderPass
 * @param offset
 * @param extent
 * @param color
 * @param depth
 * @param stencil
 * @return
 */
bool RHI::RenderPass::BeginRenderPass(Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil)
{
	assert(!m_bActive);
	assert(m_iCurrentSubpass == 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	if (0 != fb.m_uFramebufferObject)
	{
		Subpass & subpass = m_aPass.front();

		unsigned int size = subpass.m_aDrawBuffers.size();

		if (size > 0)
		{
			glDrawBuffers(size, subpass.m_aDrawBuffers.data());
		}
	}

	glViewport(offset.x, offset.y, extent.x, extent.y);
	glClearColor(color.x, color.y, color.z, color.w);
	glClearDepthf(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_bActive = true;

	return(true);
}

/**
 * @brief Pass::EndRenderPass
 */
void RHI::RenderPass::EndRenderPass(void)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass == (m_aPass.size() - 1));

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	m_bActive = false;
	m_iCurrentSubpass = 0;
}

/**
 * @brief Pass::NextSubpass
 */
void RHI::RenderPass::NextSubpass(void)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass < m_aPass.size());

///	glDrawBuffer(GL_BACK);

	++m_iCurrentSubpass;

	{
		Subpass & subpass = m_aPass[m_iCurrentSubpass];

		unsigned int size = subpass.m_aDrawBuffers.size();

		if (size > 0)
		{
			glDrawBuffers(size, subpass.m_aDrawBuffers.data());
		}
	}
}

#endif // HAVE_OPENGL

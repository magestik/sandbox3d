#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief Default constructor
 */
RHI::RenderPass::RenderPass(void)
: m_aPass()
, m_uCurrentSubpass(0)
, m_bDefaultFramebuffer(true)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Move constructor
 * @param pass
 */
RHI::RenderPass::RenderPass(RenderPass && pass)
: m_aPass(std::move(pass.m_aPass))
, m_uCurrentSubpass(pass.m_uCurrentSubpass)
, m_bDefaultFramebuffer(pass.m_bDefaultFramebuffer)
, m_bActive(pass.m_bActive)
{
	pass.m_uCurrentSubpass = 0;
	pass.m_bDefaultFramebuffer = true;
	pass.m_bActive = false;
}

/**
 * @brief RHI::RenderPass::RenderPass
 * @param pass
 */
RHI::RenderPass::RenderPass(const SubpassDescription & desc)
{
	m_aPass.push_back(Subpass(desc.aColorAttachments));
}

/**
 * @brief Constructor
 * @param aPassDesc
 */
RHI::RenderPass::RenderPass(const std::vector<SubpassDescription> & aPassDesc)
: m_aPass()
, m_uCurrentSubpass(0)
, m_bDefaultFramebuffer(true)
, m_bActive(false)
{
	for (const SubpassDescription & desc : aPassDesc)
	{
		m_aPass.push_back(Subpass(desc.aColorAttachments));
	}
}

/**
 * @brief Destructor
 */
RHI::RenderPass::~RenderPass(void)
{
	// ...
}

/**
 * @brief Move assignment
 * @param pass
 * @return
 */
RHI::RenderPass & RHI::RenderPass::operator=(RenderPass && pass)
{
	m_aPass = std::move(pass.m_aPass);

	m_uCurrentSubpass = pass.m_uCurrentSubpass;
	pass.m_uCurrentSubpass = 0;

	m_bDefaultFramebuffer = pass.m_bDefaultFramebuffer;
	pass.m_bDefaultFramebuffer = true;

	m_bActive = pass.m_bActive;
	pass.m_bActive = false;

	return(*this);
}

/**
 * @brief RHI::RenderPass::Begin
 * @param defaultFramebuffer
 * @return
 */
bool RHI::RenderPass::Begin(bool defaultFramebuffer)
{
	assert(!m_bActive);

	m_bDefaultFramebuffer = defaultFramebuffer;

	m_bActive = true;

	SetCurrent(0);

	return(true);
}

/**
 * @brief RHI::RenderPass::End
 */
void RHI::RenderPass::End(void)
{
	assert(m_bActive);
	assert(m_uCurrentSubpass == (m_aPass.size() - 1));

	glDrawBuffer(GL_BACK);

	m_bActive = false;
}

/**
 * @brief RHI::RenderPass::Next
 */
void RHI::RenderPass::Next(void)
{
	assert(m_bActive);
	assert(m_uCurrentSubpass < m_aPass.size());

	SetCurrent(m_uCurrentSubpass + 1);
}

/**
 * @brief RHI::RenderPass::SetCurrent
 */
void RHI::RenderPass::SetCurrent(unsigned int current)
{
	m_uCurrentSubpass = current;

	if (!m_bDefaultFramebuffer)
	{
		Subpass & subpass = m_aPass[m_uCurrentSubpass];

		unsigned int size = subpass.m_aDrawBuffers.size();

		if (size > 0)
		{
			glDrawBuffers(size, subpass.m_aDrawBuffers.data());
		}
	}
	else
	{
		glDrawBuffer(GL_BACK);
	}
}

#endif // HAVE_OPENGL

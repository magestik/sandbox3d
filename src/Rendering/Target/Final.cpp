#include "Final.h"

#include "../utils.inl"

/**
 * @brief Final::Final
 */
Final::Final(void)
: m_uObject(0)
{
	// ...
}

/**
 * @brief Final::~Final
 */
Final::~Final(void)
{
	// ...
}

/**
 * @brief Final::init
 * @param pColorTexture
 * @param pDepthTexture
 * @return
 */
bool Final::init(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture)
{
	glGenFramebuffers(1, &m_uObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColorTexture->GetObject(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, pDepthTexture->GetObject(), 0);

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief Final::free
 */
void Final::free(void)
{
	glDeleteFramebuffers(1, &m_uObject);
}

/**
 * @brief Final::enable
 * @return
 */
bool Final::enable(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);

	glDepthMask(GL_FALSE);

	return(true);
}

/**
 * @brief Final::disable
 * @return
 */
bool Final::disable()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);

	return(true);
}

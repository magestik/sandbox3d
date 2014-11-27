#include "LightAccumBuffer.h"

#include "../utils.inl"

/**
 * @brief LightAccumBuffer::LightAccumBuffer
 */
LightAccumBuffer::LightAccumBuffer()
: m_uObject(0)
{
	// ...
}

/**
 * @brief LightAccumBuffer::~LightAccumBuffer
 */
LightAccumBuffer::~LightAccumBuffer()
{
	// ...
}

/**
 * @brief LightAccumBuffer::init
 * @param pTexture
 * @return
 */
bool LightAccumBuffer::init(const GPU::Texture<GL_TEXTURE_2D> * pTexture)
{
	glGenFramebuffers(1, &m_uObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->GetObject(), 0);

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief LightAccumBuffer::free
 * @return
 */
void LightAccumBuffer::free(void)
{
	glDeleteFramebuffers(1, &m_uObject);
}

/**
 * @brief LightAccumBuffer::enable
 * @return
 */
bool LightAccumBuffer::enable(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glDepthMask(GL_FALSE);

	return(true);
}

/**
 * @brief LightAccumBuffer::disable
 * @return
 */
bool LightAccumBuffer::disable(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDepthMask(GL_TRUE);

	return(true);
}

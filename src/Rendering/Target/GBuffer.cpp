#include "GBuffer.h"

#include "../utils.inl"

#include <assert.h>

/**
 * @brief GBuffer::GBuffer
 */
GBuffer::GBuffer()
: m_uObject(0)
{
	// ...
}

/**
 * @brief GBuffer::~GBuffer
 */
GBuffer::~GBuffer()
{
	// ...
}

/**
 * @brief GBuffer::init
 * @param pDepthTexture
 * @return
 */
bool GBuffer::init(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture)
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
 * @brief GBuffer::free
 */
void GBuffer::free()
{
	glDeleteFramebuffers(1, &m_uObject);
}

/**
 * @brief GBuffer::enable
 * @return
 */
bool GBuffer::begin(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return(true);
}

/**
 * @brief GBuffer::disable
 * @return
 */
bool GBuffer::end(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDisable(GL_DEPTH_TEST);

	return(true);
}

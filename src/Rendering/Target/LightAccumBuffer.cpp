#include "LightAccumBuffer.h"

#include "../utils.inl"

/**
 * @brief LightAccumBuffer::LightAccumBuffer
 */
LightAccumBuffer::LightAccumBuffer()
: Pass()
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
bool LightAccumBuffer::init(const GPU::Texture<GL_TEXTURE_2D> * pTextureDiffuse, const GPU::Texture<GL_TEXTURE_2D> * pTextureSpecular)
{
	glGenFramebuffers(1, &m_uFramebufferObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTextureDiffuse->GetObject(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pTextureSpecular->GetObject(), 0);

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
	glDeleteFramebuffers(1, &m_uFramebufferObject);
}

/**
 * @brief LightAccumBuffer::enable
 * @return
 */
bool LightAccumBuffer::begin(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

	GLenum buffers [] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	return(true);
}

/**
 * @brief LightAccumBuffer::disable
 * @return
 */
bool LightAccumBuffer::end(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);

	return(true);
}

#include "ShadowMap.h"

#include "../utils.inl"

/**
 * @brief ShadowMap::ShadowMap
 */
ShadowMap::ShadowMap()
: m_uWidth(1280)
, m_uHeight(720)
, m_matProjection(1.0f)
{
	updateProjection();
}

/**
 * @brief ShadowMap::~ShadowMap
 */
ShadowMap::~ShadowMap()
{
	// ...
}

/**
 * @brief ShadowMap::Initialize
 * @return
 */
bool ShadowMap::Initialize(void)
{
	glGenFramebuffers(1, &m_uObject);
	glGenTextures(1, &m_uTexture);

	Resize(1280, 720);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief GBuffer::Release
 */
void ShadowMap::Release(void)
{
	glDeleteFramebuffers(1, &m_uObject);
	glDeleteTextures(1, &m_uTexture);
}

/**
 * @brief GBuffer::Initialize
 * @param width
 * @param height
 * @return
 */
bool ShadowMap::Resize(unsigned int width, unsigned height)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	glBindTexture(GL_TEXTURE_2D, m_uTexture);

	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uTexture, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_uWidth = width;
	m_uHeight = height;

	updateProjection();

	return true;
}

/**
 * @brief ShadowMap::updateProjection
 */
void ShadowMap::updateProjection()
{
	float ratio = m_uWidth/(float)m_uHeight;
	m_matProjection = _perspective(75.0f, ratio, 1.0f, 1000.0f); // FIXME : spot light 75° hardcoded
}

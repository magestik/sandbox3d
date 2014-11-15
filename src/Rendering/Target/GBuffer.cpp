#include "GBuffer.h"

#include "../utils.inl"

/**
 * @brief GBuffer::GBuffer
 */
GBuffer::GBuffer()
: m_uWidth(1280)
, m_uHeight(720)
, m_matProjection(1.0f)
{
	updateProjection();
}

/**
 * @brief GBuffer::~GBuffer
 */
GBuffer::~GBuffer()
{
	// ...
}

/**
 * @brief GBuffer::Initialize
 * @return
 */
bool GBuffer::Initialize(void)
{
	glGenFramebuffers(1, &m_uObject);
	glGenTextures(ETextureType::COUNT, m_uTexture);

	Resize(m_uWidth, m_uHeight);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief GBuffer::Release
 */
void GBuffer::Release(void)
{
	glDeleteFramebuffers(1, &m_uObject);
	glDeleteTextures(ETextureType::COUNT, m_uTexture);
}

/**
 * @brief GBuffer::Initialize
 * @param width
 * @param height
 * @return
 */
bool GBuffer::Resize(unsigned int width, unsigned height)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	// Position
	{
		glBindTexture(GL_TEXTURE_2D, m_uTexture[POSITION]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + POSITION, GL_TEXTURE_2D, m_uTexture[POSITION], 0);
	}

	// Diffuse
	{
		glBindTexture(GL_TEXTURE_2D, m_uTexture[DIFFUSE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + DIFFUSE, GL_TEXTURE_2D, m_uTexture[DIFFUSE], 0);
	}

	// Normal
	{
		glBindTexture(GL_TEXTURE_2D, m_uTexture[NORMAL]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + NORMAL, GL_TEXTURE_2D, m_uTexture[NORMAL], 0);
	}

	// Depth Buffer
	{
		glBindTexture(GL_TEXTURE_2D, m_uTexture[DEPTH]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uTexture[DEPTH], 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_uWidth = width;
	m_uHeight = height;

	updateProjection();

	return true;
}

/**
 * @brief GBuffer::updateProjection
 */
void GBuffer::updateProjection()
{
	float ratio = m_uWidth/(float)m_uHeight;
	m_matProjection = _perspective(75.0f, ratio, 1.0f, 1000.0f);
}

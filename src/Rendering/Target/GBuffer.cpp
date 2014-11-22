#include "GBuffer.h"

#include "../utils.inl"

#include <assert.h>

/**
 * @brief GBuffer::GBuffer
 */
GBuffer::GBuffer()
: m_uWidth(1280)
, m_uHeight(720)
, m_matProjection(1.0f)
{
	glGenFramebuffers(1, &m_uObject);
	Resize(m_uWidth, m_uHeight);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief GBuffer::~GBuffer
 */
GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &m_uObject);
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
		m_texture[POSITION].init<GL_RGB16F>(width, height);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + POSITION, GL_TEXTURE_2D, m_texture[POSITION].GetObject(), 0);
	}

	// Diffuse
	{
		m_texture[DIFFUSE].init<GL_RGB16F>(width, height);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + DIFFUSE, GL_TEXTURE_2D, m_texture[DIFFUSE].GetObject(), 0);
	}

	// Normal
	{
		m_texture[NORMAL].init<GL_RGB16F>(width, height);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + NORMAL, GL_TEXTURE_2D, m_texture[NORMAL].GetObject(), 0);
	}

	// Depth Buffer
	{
		m_texture[DEPTH].init<GL_DEPTH_COMPONENT32F>(width, height);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture[DEPTH].GetObject(), 0);
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

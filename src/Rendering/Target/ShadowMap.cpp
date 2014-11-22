#include "ShadowMap.h"

#include "../utils.inl"

#include <assert.h>

/**
 * @brief ShadowMap::ShadowMap
 */
ShadowMap::ShadowMap()
: m_uWidth(1024)
, m_uHeight(1024)
, m_matProjection(1.0f)
{
	glGenFramebuffers(1, &m_uObject);
	Resize(m_uWidth, m_uHeight);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief ShadowMap::~ShadowMap
 */
ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &m_uObject);
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

	glBindTexture(GL_TEXTURE_2D, m_texture.GetObject());

	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//float color [4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.GetObject(), 0);
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
	// FIXME : spot light 45° hardcoded
	m_matProjection = _perspective(45.0f, 1.0f, 1.0f, 100.0f);
}

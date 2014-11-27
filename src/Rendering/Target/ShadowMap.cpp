#include "ShadowMap.h"

#include "../utils.inl"

#include <assert.h>

/**
 * @brief ShadowMap::ShadowMap
 */
ShadowMap::ShadowMap(void)
: m_uObject(0)
, m_matProjection(1.0f)
{
	// ...
}

/**
 * @brief ShadowMap::~ShadowMap
 */
ShadowMap::~ShadowMap(void)
{
	// ...
}

/**
 * @brief ShadowMap::init
 * @param width
 * @param height
 * @return
 */
bool ShadowMap::init(unsigned int width, unsigned height)
{
	glGenFramebuffers(1, &m_uObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	{
		m_texture.init<GL_DEPTH_COMPONENT32F>(width, height);

		glBindTexture(GL_TEXTURE_2D, m_texture.GetObject());
		//float color [4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.GetObject(), 0);
	}

	m_matProjection = _perspective(45.0f, 1.0f, 1.0f, 100.0f); // FIXME : spot light 45° hardcoded

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return (status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief ShadowMap::free
 */
void ShadowMap::free(void)
{
	glDeleteFramebuffers(1, &m_uObject);
}

/**
 * @brief ShadowMap::enable
 * @return
 */
bool ShadowMap::enable(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);
	glDrawBuffer(GL_NONE);

	glEnable(GL_DEPTH_TEST);

	glPolygonOffset(10.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	return(true);
}

/**
 * @brief ShadowMap::disable
 * @return
 */
bool ShadowMap::disable(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_POLYGON_OFFSET_FILL);

	return(true);
}

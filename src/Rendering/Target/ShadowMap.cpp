#include "ShadowMap.h"

#include "../utils.inl"

#include "../Rendering.h"

#include <assert.h>

/**
 * @brief ShadowMap::ShadowMap
 */
ShadowMap::ShadowMap(const Pipeline * pipeline)
: Subpass(pipeline)
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
    glGenFramebuffers(1, &m_uFramebufferObject);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

    m_texture.init<GL_DEPTH_COMPONENT32F>(width, height);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.GetObject(), 0);

    //m_matProjection = _perspective(45.0f, 1.0f, 1.0f, 100.0f); // FIXME : spot light 45° hardcoded
    m_matProjection = _ortho(-20.0f, 20.0f, -20.0f, 20.0f, -10.0f, 100.0f);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    m_aDrawBuffers.push_back(GL_NONE);

    return (status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief ShadowMap::free
 */
void ShadowMap::free(void)
{
    glDeleteFramebuffers(1, &m_uFramebufferObject);
    m_uFramebufferObject = 0;
}

#include "AverageLuminance.h"

#include "../utils.inl"

#include "../Rendering.h"

/**
 * @brief AverageLuminance::AverageLuminance
 */
AverageLuminance::AverageLuminance(void)
: m_uCurrentObject(0)
, m_pShader_convert(nullptr)
, m_pShader_mean(nullptr)
, m_pCurrentShader(nullptr)
{
    m_uObject[0] = 0;
    m_uObject[1] = 0;
}

/**
 * @brief AverageLuminance::~AverageLuminance
 */
AverageLuminance::~AverageLuminance(void)
{
    // ...
}

/**
 * @brief AverageLuminance::init
 * @param pTexture1
 * @param pTexture2
 * @return
 */
bool AverageLuminance::init(const GPU::Texture<GL_TEXTURE_2D> * pTexture1, const GPU::Texture<GL_TEXTURE_2D> * pTexture2)
{
    m_pShader_convert = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_luminance.frag"]);
    m_pShader_mean = new Shader(g_VertexShaders["fullscreen_scaled.vert"], g_FragmentShaders["fullscreen_luminance_mean.frag"]);

    glGenFramebuffers(2, m_uObject);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[0]);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture1->GetObject(), 0);

    GLenum status1 = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[1]);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture2->GetObject(), 0);

    GLenum status2 = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return(status1 == GL_FRAMEBUFFER_COMPLETE && status2 == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief AverageLuminance::free
 */
void AverageLuminance::free(void)
{
    delete m_pShader_convert;
    m_pShader_convert = nullptr;

    delete m_pShader_mean;
    m_pShader_mean = nullptr;

    glDeleteFramebuffers(2, m_uObject);
    m_uObject[0] = 0;
    m_uObject[1] = 0;
}

/**
 * @brief AverageLuminance::enable
 * @return
 */
bool AverageLuminance::begin(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[0]);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glDepthMask(GL_FALSE);

    m_pCurrentShader = m_pShader_convert;
    m_pCurrentShader->SetAsCurrent();

    m_uCurrentObject = 0;

    return(true);
}

/**
 * @brief AverageLuminance::disable
 * @return
 */
bool AverageLuminance::end(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    glDepthMask(GL_TRUE);

    m_fSumLog = 0.0f;

    glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uObject[m_uCurrentObject]); // read last written FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, (void*)&m_fSumLog);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    m_uCurrentObject = 0;

    glUseProgram(0);
    m_pCurrentShader = nullptr;

    return(true);
}

/**
 * @brief AverageLuminance::next
 */
unsigned int AverageLuminance::next(void)
{
    unsigned int source_texture = m_uCurrentObject;

    if (nullptr != m_pCurrentShader)
    {
        m_uCurrentObject = 1 - m_uCurrentObject;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[m_uCurrentObject]);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if (m_pShader_mean != m_pCurrentShader)
        {
            m_pCurrentShader = m_pShader_mean;
            m_pCurrentShader->SetAsCurrent();
        }
    }

    return(source_texture);
}

/**
 * @brief AverageLuminance::getValue
 * @param N
 * @return
 */
float AverageLuminance::getValue(void)
{
    return(exp(m_fSumLog));
}

#include "GBuffer.h"

#include "../utils.inl"

#include "../Rendering.h"

/**
 * @brief GBuffer::GBuffer
 */
GBuffer::GBuffer()
: m_uObject(0)
, m_pShader_simple(nullptr)
, m_pShader_normalmap(nullptr)
, m_pCurrentShader(nullptr)
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
    m_pShader_simple    = new Shader(g_VertexShaders["geometry_pass.vert"], g_FragmentShaders["geometry_pass.frag"]);
    m_pShader_normalmap	= new Shader(g_VertexShaders["geometry_normalmap_pass.vert"], g_FragmentShaders["geometry_normalmap_pass.frag"]);

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
void GBuffer::free(void)
{
    delete m_pShader_simple;
    m_pShader_simple = nullptr;

    delete m_pShader_normalmap;
    m_pShader_normalmap = nullptr;

	glDeleteFramebuffers(1, &m_uObject);
    m_uObject = 0;
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

    m_pCurrentShader = m_pShader_simple;
    m_pCurrentShader->SetAsCurrent();

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

    glUseProgram(0);
    m_pCurrentShader = nullptr;

    return(true);
}

/**
 * @brief GBuffer::enable_normalmapping
 */
void GBuffer::enable_normalmapping(void)
{
    if (nullptr != m_pCurrentShader && m_pCurrentShader != m_pShader_normalmap)
    {
        m_pCurrentShader = m_pShader_normalmap;
        m_pCurrentShader->SetAsCurrent();
    }
}

/**
 * @brief GBuffer::enable_normalmapping
 */
void GBuffer::disable_normalmapping(void)
{
    if (nullptr != m_pCurrentShader && m_pCurrentShader != m_pShader_simple)
    {
        m_pCurrentShader = m_pShader_simple;
        m_pCurrentShader->SetAsCurrent();
    }
}

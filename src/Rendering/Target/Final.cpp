#include "Final.h"

#include "../utils.inl"

#include "../Rendering.h"

/**
 * @brief Final::Final
 */
Final::Final(void)
: m_uObject(0)
, m_pShader(nullptr)
{
	// ...
}

/**
 * @brief Final::~Final
 */
Final::~Final(void)
{
	// ...
}

/**
 * @brief Final::init
 * @param pColorTexture
 * @param pDepthTexture
 * @return
 */
bool Final::init(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture)
{
    m_pShader = new Shader(g_VertexShaders["full.vert"], g_FragmentShaders["full.frag"]);

	glGenFramebuffers(1, &m_uObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColorTexture->GetObject(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, pDepthTexture->GetObject(), 0);

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief Final::free
 */
void Final::free(void)
{
    delete m_pShader;
    m_pShader = nullptr;

	glDeleteFramebuffers(1, &m_uObject);
    m_uObject = 0;
}

/**
 * @brief Final::enable
 * @return
 */
bool Final::begin(const vec4 & clearColor)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);

	glDepthMask(GL_FALSE);

	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

    m_pShader->SetAsCurrent();

	return(true);
}

/**
 * @brief Final::disable
 * @return
 */
bool Final::end(void)
{
    glUseProgram(0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);

	return(true);
}

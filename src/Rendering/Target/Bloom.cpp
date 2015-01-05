#include "Bloom.h"

#include "../utils.inl"

#include "../Rendering.h"

/**
 * @brief Bloom::Bloom
 */
Bloom::Bloom(void)
: m_uCurrentObject(0)
, m_pShader_bright(nullptr)
, m_pCurrentShader(nullptr)
{
	m_uObject[0] = 0;
	m_uObject[1] = 0;

	m_apShader_blur[0] = nullptr;
	m_apShader_blur[1] = nullptr;
}

/**
 * @brief Bloom::~Bloom
 */
Bloom::~Bloom()
{
	// ...
}

/**
 * @brief Bloom::init
 * @param pTexture1
 * @param pTexture2
 * @return
 */
bool Bloom::init(const GPU::Texture<GL_TEXTURE_2D> * pTexture1, const GPU::Texture<GL_TEXTURE_2D> * pTexture2)
{
	m_pShader_bright = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_bright.frag"]);
	m_apShader_blur[0] = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["gaussian_blur_h.frag"]);
	m_apShader_blur[1] = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["gaussian_blur_v.frag"]);

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
 * @brief Bloom::free
 */
void Bloom::free(void)
{
	delete m_pShader_bright;
	m_pShader_bright = nullptr;

	delete m_apShader_blur[0];
	m_apShader_blur[0] = nullptr;

	delete m_apShader_blur[1];
	m_apShader_blur[1] = nullptr;

	glDeleteBuffers(2, m_uObject);
	m_uObject[0] = 0;
	m_uObject[1] = 0;
}

/**
 * @brief Bloom::begin
 * @return
 */
bool Bloom::begin(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[0]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glDepthMask(GL_FALSE);

	m_pCurrentShader = m_pShader_bright;
	m_pCurrentShader->SetAsCurrent();

	m_uCurrentObject = 0;

	return(true);
}

/**
 * @brief Bloom::end
 * @return
 */
bool Bloom::end(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glDepthMask(GL_TRUE);

	m_uCurrentObject = 0;

	glUseProgram(0);
	m_pCurrentShader = nullptr;

	return(true);
}

/**
 * @brief Bloom::prepare_blur_h
 */
void Bloom::prepare_blur_h(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[1]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glDepthMask(GL_FALSE);

	m_pCurrentShader = m_apShader_blur[0];
	m_pCurrentShader->SetAsCurrent();

	m_uCurrentObject = 1;
}

/**
 * @brief Bloom::prepare_blur_v
 */
void Bloom::prepare_blur_v(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uObject[0]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glDepthMask(GL_FALSE);

	m_pCurrentShader = m_apShader_blur[1];
	m_pCurrentShader->SetAsCurrent();

	m_uCurrentObject = 0;
}

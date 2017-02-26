#include "AverageLuminance.h"

#include "../utils.inl"

#include "../Rendering.h"

/**
 * @brief AverageLuminance::AverageLuminance
 */
AverageLuminance::AverageLuminance(void)
: Subpass()
, m_uCurrentObject(0)
, m_fSumLog(0.0f)
, m_fMax(0.0f)
{
	// ...
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
	glGenFramebuffers(1, &m_uFramebufferObject);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture1->GetObject(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pTexture2->GetObject(), 0);

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_aDrawBuffers.push_back(GL_COLOR_ATTACHMENT0);
	m_aDrawBuffers.push_back(GL_COLOR_ATTACHMENT1);

	return(status == GL_FRAMEBUFFER_COMPLETE);
}

/**
 * @brief AverageLuminance::free
 */
void AverageLuminance::free(void)
{
	glDeleteFramebuffers(1, &m_uFramebufferObject);
	m_uFramebufferObject = 0;
}

/**
 * @brief AverageLuminance::enable
 * @return
 */
bool AverageLuminance::begin(void)
{
///	Subpass::Begin();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

	m_uCurrentObject = 0;

	return(true);
}

/**
 * @brief AverageLuminance::disable
 * @return
 */
bool AverageLuminance::end(void)
{
///	Subpass::End();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	float v [2];

	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uFramebufferObject); // read last written FBO
	glReadBuffer(GL_COLOR_ATTACHMENT0 + m_uCurrentObject);
	glReadPixels(0, 0, 1, 1, GL_RG, GL_FLOAT, (void*)v);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	m_fSumLog = v[0];
	m_fMax = v[1];

	m_uCurrentObject = 0;

	return(true);
}

/**
 * @brief AverageLuminance::next
 */
unsigned int AverageLuminance::next(void)
{
	unsigned int source_texture = m_uCurrentObject;

	m_uCurrentObject = 1 - m_uCurrentObject;

	glDrawBuffer(GL_COLOR_ATTACHMENT0 + m_uCurrentObject);

	return(source_texture);
}

/**
 * @brief AverageLuminance::getValue
 * @param N
 * @return
 */
float AverageLuminance::getAverage(void)
{
	return(exp(m_fSumLog));
}

/**
 * @brief AverageLuminance::getMax2
 * @return
 */
float AverageLuminance::getMax2()
{
	return(m_fMax*m_fMax);
}

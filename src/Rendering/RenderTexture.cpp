#include "RenderTexture.h"

/**
 * @brief RenderTexture::RenderTexture
 */
RenderTexture::RenderTexture(void)
: m_pTexture(nullptr)
, m_eFormat(0)
, m_uSizeDivisor(0)
{
	// ...
}

/**
 * @brief RenderTexture::RenderTexture
 * @param f
 * @param r
 */
RenderTexture::RenderTexture(GLenum f, unsigned int r)
: m_eFormat(f)
, m_uSizeDivisor(r)
{
	m_pTexture = new GPU::Texture<GL_TEXTURE_2D>();
}

/**
 * @brief RenderTexture::resize
 * @param width
 * @param height
 */
void RenderTexture::resize(int width, int height)
{
	width	/= m_uSizeDivisor;
	height	/= m_uSizeDivisor;

	switch (m_eFormat)
	{
		case GL_RGBA16F:
		{
			m_pTexture->init<GL_RGBA16F>(width, height);
		}
		break;

		case GL_RGBA32F:
		{
			m_pTexture->init<GL_RGBA32F>(width, height);
		}
		break;

		case GL_R32UI:
		{
			m_pTexture->init<GL_R32UI>(width, height);
		}
		break;

		case GL_RGB10_A2:
		{
			m_pTexture->init<GL_RGB10_A2>(width, height);
		}
		break;

		case GL_RGB10_A2UI:
		{
			m_pTexture->init<GL_RGB10_A2UI>(width, height);
		}
		break;

		case GL_R11F_G11F_B10F:
		{
			m_pTexture->init<GL_R11F_G11F_B10F>(width, height);
		}
		break;

		case GL_DEPTH_COMPONENT16:
		{
			m_pTexture->init<GL_DEPTH_COMPONENT16>(width, height);
		}
		break;

		case GL_DEPTH_COMPONENT24:
		{
			m_pTexture->init<GL_DEPTH_COMPONENT24>(width, height);
		}
		break;

		case GL_DEPTH_COMPONENT32F:
		{
			m_pTexture->init<GL_DEPTH_COMPONENT32F>(width, height);
		}
		break;

		case GL_DEPTH24_STENCIL8:
		{
			m_pTexture->init<GL_DEPTH24_STENCIL8>(width, height);
		}
		break;

		case GL_DEPTH32F_STENCIL8:
		{
			m_pTexture->init<GL_DEPTH32F_STENCIL8>(width, height);
		}
		break;

		case GL_STENCIL_INDEX8:
		{
			m_pTexture->init<GL_DEPTH32F_STENCIL8>(width, height);
		}
		break;

		default:
		{
			assert(false);
		}
	}
}

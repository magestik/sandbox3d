#pragma once

#include <GPU.h>

#include <assert.h>

struct RenderTexture
{
	RenderTexture() : format(0), size_divisor(0), pTexture(nullptr)
	{

	}

	RenderTexture(GLenum f, unsigned int r = 1) : format(f), size_divisor(r)
	{
		pTexture = new GPU::Texture<GL_TEXTURE_2D>();
	}

	void resize(int width, int height)
	{
		width	/= size_divisor;
		height	/= size_divisor;

		switch (format)
		{
			case GL_RGBA16F:
			{
				pTexture->init<GL_RGBA16F>(width, height);
			}
			break;

			case GL_RGBA32F:
			{
				pTexture->init<GL_RGBA32F>(width, height);
			}
			break;

			case GL_RGB10_A2:
			{
				pTexture->init<GL_RGB10_A2>(width, height);
			}
			break;

			case GL_RGB10_A2UI:
			{
				pTexture->init<GL_RGB10_A2UI>(width, height);
			}
			break;

			case GL_R11F_G11F_B10F:
			{
				pTexture->init<GL_R11F_G11F_B10F>(width, height);
			}
			break;

			case GL_DEPTH_COMPONENT16:
			{
				pTexture->init<GL_DEPTH_COMPONENT16>(width, height);
			}
			break;

			case GL_DEPTH_COMPONENT24:
			{
				pTexture->init<GL_DEPTH_COMPONENT24>(width, height);
			}
			break;

			case GL_DEPTH_COMPONENT32F:
			{
				pTexture->init<GL_DEPTH_COMPONENT32F>(width, height);
			}
			break;

			case GL_DEPTH24_STENCIL8:
			{
				pTexture->init<GL_DEPTH24_STENCIL8>(width, height);
			}
			break;

			case GL_DEPTH32F_STENCIL8:
			{
				pTexture->init<GL_DEPTH32F_STENCIL8>(width, height);
			}
			break;

			case GL_STENCIL_INDEX8:
			{
				pTexture->init<GL_DEPTH32F_STENCIL8>(width, height);
			}
			break;

			default:
			{
				assert(false);
			}
		}
	}

	const GPU::Texture<GL_TEXTURE_2D> * getTexture(void) const
	{
		return(pTexture);
	}

private:
	GPU::Texture<GL_TEXTURE_2D> * pTexture;
	GLenum format;
	unsigned int size_divisor;
};

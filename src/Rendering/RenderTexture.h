#pragma once

#include <GPU.h>

#include <assert.h>

class RenderTexture
{

public:

	explicit	RenderTexture	(void);
	explicit	RenderTexture	(GLenum f, unsigned int r = 1);

	void		resize			(int width, int height);

	const GPU::Texture<GL_TEXTURE_2D> * getTexture(void) const
	{
		return(m_pTexture);
	}

private:

	GPU::Texture<GL_TEXTURE_2D> * m_pTexture;
	GLenum m_eFormat;
	unsigned int m_uSizeDivisor;

};

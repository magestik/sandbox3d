#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class Shader;

class GBuffer
{
public:

	enum ETextureType
	{
		POSITION	= 0,
		DIFFUSE		= 1,
		NORMAL		= 2,
		TEXCOORD	= 3,
		COUNT		= 4
	};

	GBuffer(void);
	~GBuffer(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture);
	void	free	(void);

	bool	begin	(void);
	bool	end		(void);

	GLuint	GetObject	(void) { return(m_uObject); }

    Shader * GetShader	(void) { return(m_pCurrentShader); }

    void enable_normalmapping   (void);
    void disable_normalmapping  (void);

private:

	GLuint m_uObject;

    Shader * m_pShader_simple;
    Shader * m_pShader_normalmap;

    Shader * m_pCurrentShader;
};

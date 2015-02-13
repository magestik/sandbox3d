#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include "Technique.h"

class Shader;

class Final : public Pass
{
public:

	Final(void);
	~Final(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture);
	void	free	(void);

	bool	begin	(const vec4 & clearColor);
	bool	end		(void);

	GLuint	GetObject(void) { return(m_uFramebufferObject); }

	Shader * GetShader(void) { return(m_pShader); }

private:

	Shader * m_pShader;
};

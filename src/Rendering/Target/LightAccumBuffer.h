#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include "Technique.h"

class LightAccumBuffer : public Pass
{
public:

	LightAccumBuffer	(void);
	~LightAccumBuffer	(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pTextureDiffuse, const GPU::Texture<GL_TEXTURE_2D> * pTextureSpecular);
	void	free	(void);

	bool	begin	(void);
	bool	end		(void);

	GLuint	GetObject(void) { return(m_uFramebufferObject); }

};

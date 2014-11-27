#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class Final
{
public:

	Final(void);
	~Final(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pColorTexture, const GPU::Texture<GL_TEXTURE_2D> * pDepthTexture);
	void	free	(void);

	bool	enable	(void);
	bool	disable	(void);

	GLuint	GetObject(void) { return(m_uObject); }

private:

	GLuint m_uObject;

};

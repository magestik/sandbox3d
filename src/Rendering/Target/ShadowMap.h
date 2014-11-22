#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class ShadowMap
{
public:

	ShadowMap(void);
	~ShadowMap(void);

	void	Release		(void);

	bool	Resize		(unsigned int width, unsigned height);

	GLuint	GetObject	(void) { return(m_uObject); }

	const GPU::Texture<GL_TEXTURE_2D> &	GetTexture	(void) { return(m_texture); }

	GLuint	GetWidth	(void) { return(m_uWidth); }
	GLuint	GetHeight	(void) { return(m_uHeight); }

	const mat4x4 &	GetProjection (void) const { return(m_matProjection); }

protected:

	void updateProjection(void);

private:

	unsigned int m_uWidth;
	unsigned int m_uHeight;

	mat4x4 m_matProjection;

	GLuint m_uObject;

	GPU::Texture<GL_TEXTURE_2D> m_texture;

};

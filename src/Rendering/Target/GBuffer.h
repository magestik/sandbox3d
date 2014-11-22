#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class GBuffer
{
public:

	enum ETextureType
	{
		POSITION	= 0,
		DIFFUSE		= 1,
		NORMAL		= 2,
		TEXCOORD	= 3,
		DEPTH		= 4,
		COUNT		= 5
	};

	GBuffer(void);
	~GBuffer(void);

	bool	Resize		(unsigned int width, unsigned height);

	GLuint	GetObject	(void) { return(m_uObject); }

	const GPU::Texture<GL_TEXTURE_2D> & GetTexture	(ETextureType type) { return(m_texture[type]); }

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

	GPU::Texture<GL_TEXTURE_2D> m_texture [COUNT];

};

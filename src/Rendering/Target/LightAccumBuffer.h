#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class LightAccumBuffer
{
public:

	LightAccumBuffer(void);
	~LightAccumBuffer(void);

	bool	Initialize	(void);
	void	Release		(void);

	bool	Resize		(unsigned int width, unsigned height);

	GLuint	GetObject	(void) { return(m_uObject); }

	GLuint	GetTexture	(void) { return(m_uTexture); }

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

	GLuint m_uTexture;

};

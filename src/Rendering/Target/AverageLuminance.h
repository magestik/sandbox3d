#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class Shader;

class AverageLuminance
{
public:

	AverageLuminance(void);
	~AverageLuminance(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pTexture1, const GPU::Texture<GL_TEXTURE_2D> * pTexture2);
	void	free	(void);

	bool	begin	(void);
	bool	end		(void);

	unsigned int    next    (void);

	float getAverage(void);
	float getMax2(void);

	GLuint	GetObject(void) { return(m_uCurrentObject); }

	Shader * GetShader(void) { return(m_pCurrentShader); }

private:

	GLuint m_uObjects [2];

	unsigned int m_uCurrentObject;

	Shader * m_pShader_convert;
	Shader * m_pShader_mean;

	Shader * m_pCurrentShader;

	float m_fSumLog;
	float m_fMax;
};

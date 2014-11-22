#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class Shader
{
public:

	explicit	Shader(GPU::Shader<GL_VERTEX_SHADER> * vs, GPU::Shader<GL_FRAGMENT_SHADER> * fs);
	virtual		~Shader(void);

	void		SetAsCurrent	(void);

	void		SetUniform		(const char * name, const mat4x4 & m);
	void		SetUniform		(const char * name, const mat3x3 & m);
	void		SetUniform		(const char * name, const mat2x2 & m);

	void		SetUniform		(const char * name, const vec4 & v);
	void		SetUniform		(const char * name, const vec3 & v);
	void		SetUniform		(const char * name, const vec2 & v);

	void		SetUniform		(const char * name, int n);
	void		SetUniform		(const char * name, float n);

	template<GLenum T>
	void SetTexture(const char * name, int unit, const GPU::Texture<T> & texture);

private:

	GPU::Shader<GL_VERTEX_SHADER> *		m_pVertexShader;
	GPU::Shader<GL_FRAGMENT_SHADER> *	m_pFragmentShader;

	GLuint m_uObject;
};

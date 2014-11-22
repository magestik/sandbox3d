#include "Shader.h"

#include <assert.h>

/**
 * @brief Shader::Shader
 */
Shader::Shader(GPU::Shader<GL_VERTEX_SHADER> * vs, GPU::Shader<GL_FRAGMENT_SHADER> * fs)
: m_pVertexShader(vs)
, m_pFragmentShader(fs)
{
	m_uObject = glCreateProgram();

	glAttachShader(m_uObject, m_pVertexShader->GetObject());
	glAttachShader(m_uObject, m_pFragmentShader->GetObject());

	glLinkProgram(m_uObject);

	int status = 0;
	glGetProgramiv(m_uObject, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		return;
	}
}

/**
 * @brief Shader::~Shader
 */
Shader::~Shader(void)
{
	glDetachShader(m_uObject, m_pVertexShader->GetObject());
	glDetachShader(m_uObject, m_pFragmentShader->GetObject());

	glDeleteProgram(m_uObject);
}

/**
 * @brief Shader::SetAsCurrent
 */
void Shader::SetAsCurrent(void)
{
	glUseProgram(m_uObject);
}

/**
 * @brief Shader::SetUniform
 * @param name
 * @param m
 */
void Shader::SetUniform(const char * name, const mat4x4 & m)
{
	GLint location = glGetUniformLocation(m_uObject, name);
//	assert(-1 != location);
	glUniformMatrix4fv(location, 1, false, (float*)&m);
}

/**
 * @brief Shader::SetUniform
 * @param name
 * @param v
 */
void Shader::SetUniform(const char * name, const vec3 & v)
{
	GLint location = glGetUniformLocation(m_uObject, name);
//	assert(-1 != location);
	glUniform3fv(location, 1, (float*)&v);
}

/**
 * @brief Shader::SetUniform
 * @param name
 * @param m
 */
void Shader::SetUniform(const char * name, int n)
{
	GLint location = glGetUniformLocation(m_uObject, name);
//	assert(-1 != location);
	glUniform1i(location, n);
}

/**
 * @brief Shader::SetTexture
 * @param name
 * @param unit
 * @param texture
 */
template<GLenum T>
void Shader::SetTexture(const char * name, int unit, const GPU::Texture<T> & texture)
{
	SetUniform(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(T, texture.GetObject());
}

template void Shader::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Shader::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Shader::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

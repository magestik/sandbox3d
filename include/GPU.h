#pragma once

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

namespace GPU
{

template<GLenum T>
class Buffer
{
public:

	explicit Buffer(void)
	{
		glGenBuffers(1, &m_uObject);
	}

	~Buffer(void)
	{
		glDeleteBuffers(1, &m_uObject);
	}

	bool allocate(size_t s, GLuint usage, void * pData = nullptr)
	{
		glBindBuffer(T, m_uObject);
		glBufferData(T, s, pData, usage);
		glBindBuffer(T, 0);
	}

	void * map(GLuint access)
	{
		glBindBuffer(T, m_uObject);
		glMapBuffer(T, access);
		glBindBuffer(T, m_uObject);
	}

	void unmap(void)
	{
		glBindBuffer(T, m_uObject);
		glUnmapBuffer(m_uObject);
		glBindBuffer(T, 0);
	}

	GLuint	GetObject (void) { return(m_uObject); }

private:

	GLuint m_uObject;
};

template<GLenum T>
class Shader
{
public:

	explicit Shader(void)
	{
		m_uObject = glCreateShader(T);
	}

	~Shader(void)
	{
		glDeleteShader(m_uObject);
	}

	bool compileFromSource(const char * source)
	{
		glShaderSource(m_uObject, 1, &source, NULL);
		glCompileShader(m_uObject);
		GLint isCompiled = 0;
		glGetShaderiv(m_uObject, GL_COMPILE_STATUS, &isCompiled);
		return(GL_TRUE == isCompiled);
	}

	GLuint	GetObject (void) { return(m_uObject); }

private:

	GLuint m_uObject;

};

template<GLenum T>
class Texture
{
public:

	explicit Texture(void)
	{
		glGenTextures(1, &m_uObject);
	}

	~Texture(void)
	{
		glDeleteTextures(1, &m_uObject);
	}

	bool allocate(GLsizei width, GLsizei height, const void * pData = nullptr)
	{
		glBindTexture(T, m_uObject);
		glTexImage2D(T, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, pData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(T, 0);
	}

	GLuint	GetObject (void) { return(m_uObject); }

private:

	GLuint m_uObject;

};

}

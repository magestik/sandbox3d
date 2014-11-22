#pragma once

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include "GPU_helper.h"

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

	GLuint	GetObject (void) const { return(m_uObject); }

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

	GLuint	GetObject (void) const { return(m_uObject); }

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

	template<GLint internalFormat>
	void init(GLsizei width, GLsizei height)
	{
		GLenum format = getFormat(internalFormat);
		GLenum type = getType(internalFormat);

		glBindTexture(GL_TEXTURE_2D, m_uObject);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	template<GLint internalFormat>
	void init(GLsizei width, GLsizei height, const Buffer<GL_PIXEL_UNPACK_BUFFER> & buffer, GLenum format, GLenum type)
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer.GetObject());

		glBindTexture(GL_TEXTURE_2D, m_uObject);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}

	GLuint	GetObject (void) const { return(m_uObject); }

private:

	GLuint m_uObject;

};

}

#include "GPU_mem.h"

#pragma once

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include "GPU_helper.h"

#define BUFFER_OFFSET(i) ((char*)NULL+(i))

namespace GPU
{

template<GLenum T>
class Buffer
{
public:

	Buffer (const Buffer<T> &) = delete;

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

	Buffer<T> & operator = (const Buffer<T> &) { /* ... */ }

	GLuint m_uObject;
};

template<GLenum T>
class Shader
{
public:

	Shader (const Shader<T> &) = delete;

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

	Shader<T> & operator = (const Shader<T> &) { /* ... */ }

	GLuint m_uObject;

};

template<GLenum T>
class Texture
{
public:

	Texture (const Texture<T> &) = delete;

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
		// Normally used to specify pixel data format/type
		// But GL implementations check what we pass even if data is NULL !
		// So we have to give acceptable USELESS parameters ...
		GLenum format = getFormat(internalFormat);
		GLenum type = getType(internalFormat);

        glBindTexture(T, m_uObject);
        glTexImage2D(T, 0, internalFormat, width, height, 0, format, type, nullptr);
        glTexParameteri(T, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(T, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(T, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(T, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(T, 0);
	}

	template<GLint internalFormat>
	void init(GLsizei width, GLsizei height, const Buffer<GL_PIXEL_UNPACK_BUFFER> & buffer, GLenum format, GLenum type)
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer.GetObject());

        glBindTexture(T, m_uObject);
        glTexImage2D(T, 0, internalFormat, width, height, 0, format, type, nullptr);
        glTexParameteri(T, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(T, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(T, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(T, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(T, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}

	GLuint	GetObject (void) const { return(m_uObject); }

private:

	Texture<T> & operator = (const Texture<T> &) { /* ... */ }

	GLuint m_uObject;

};

}

#include "GPU_mem.h"

#pragma once

#include <GL3/gl3.h>
#include <GPU.h>
#include <string.h>

namespace GPU
{

/**
 * @brief malloc
 * @param buffer
 * @param s
 * @param pData
 */
template<GLenum T>
inline void realloc(const Buffer<T> & buffer, size_t s, const void * pData = nullptr)
{
	glBindBuffer(T, buffer.GetObject());
	glBufferData(T, s, pData, GL_STATIC_DRAW);
	glBindBuffer(T, 0);
}

/**
 * @brief malloc
 * @param buffer
 * @param s
 * @param usage
 * @param pData
 */
template<GLenum T>
inline void realloc(const Buffer<T> & buffer, size_t s, GLenum usage, const void * pData = nullptr)
{
	glBindBuffer(T, buffer.GetObject());
	glBufferData(T, s, pData, usage);
	glBindBuffer(T, 0);
}

/**
 * @brief mmap
 * @param access
 * @return
 */
template<GLenum T>
void * mmap(const Buffer<T> & buffer, GLuint access)
{
	glBindBuffer(T, buffer.GetObject());
	void * ptr = glMapBuffer(T, access);
	glBindBuffer(T, 0);
	return(ptr);
}

/**
 * @brief mmap
 * @param access
 * @return
 */
template<GLenum T>
void * munmap(const Buffer<T> & buffer)
{
	glBindBuffer(T, buffer.GetObject());
	glUnmapBuffer(T);
	glBindBuffer(T, 0);
}

/**
 * @brief memcpy
 * @param buffer
 * @return
 */
template<GLenum T>
void memcpy(const Buffer<T> & buffer, const void * pData, size_t s)
{
	glBindBuffer(T, buffer.GetObject());
	void * ptr = glMapBuffer(T, GL_WRITE_ONLY);
	::memcpy(ptr, pData, s);
	glUnmapBuffer(T);
	glBindBuffer(T, 0);
}

/**
 * @brief memcpy
 * @param buffer
 * @return
 */
template<GLenum T>
void memcpy(const void * pData, const Buffer<T> & buffer, size_t s)
{
	glBindBuffer(T, buffer.GetObject());
	const void * ptr = glMapBuffer(T, GL_READ_ONLY);
	::memcpy(pData, ptr, s);
	glUnmapBuffer(T);
	glBindBuffer(T, 0);
}

}

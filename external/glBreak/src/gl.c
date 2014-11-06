#include "structs.h"

gl_ptrs gl;

/**
 * @brief glDebugMessageControl
 * @param source
 * @param type
 * @param severity
 * @param count
 * @param ids
 * @param enabled
 */
void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{

}

/**
 * @brief glDebugMessageInsert
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param buf
 */
void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)
{

}

/**
 * @brief glDebugMessageCallback
 * @param callback
 * @param userParam
 */
void glDebugMessageCallback(GLDEBUGPROC callback, const void * userParam)
{

}

/**
 * @brief glGetDebugMessageLog
 * @param count
 * @param bufSize
 * @param sources
 * @param types
 * @param ids
 * @param severities
 * @param lengths
 * @param messageLog
 * @return
 */
GLuint glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum *severities, GLsizei *lengths, GLchar * messageLog)
{
	return(0);
}

/**
 * @brief glPushDebugGroup
 * @param source
 * @param id
 * @param length
 * @param message
 */
void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message)
{

}

/**
 * @brief glPopDebugGroup
 */
void glPopDebugGroup(void)
{

}

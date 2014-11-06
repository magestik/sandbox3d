#pragma once

extern void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
extern void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
extern void glDebugMessageCallback(GLDEBUGPROC callback, const void * userParam);
extern GLuint glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum *severities, GLsizei *lengths, GLchar * messageLog);
extern void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message);
extern void glPopDebugGroup(void);

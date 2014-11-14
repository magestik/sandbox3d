#include "structs.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void glBreak_initialize(void) __attribute__((constructor));
void glBreak_release(void) __attribute__((destructor));
void glBreak_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

static void * lib = NULL;

/**
 * @brief glBreak_initialize
 */
void glBreak_initialize(void)
{
	lib = dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);

	memset(&glX, 0x0, sizeof(glX_ptrs));

	glX.GetProcAddress			= dlsym(lib, "glXGetProcAddress");
	glX.CreateContext			= dlsym(lib, "glXCreateContext");
	glX.MakeCurrent				= dlsym(lib, "glXMakeCurrent");
	glX.MakeContextCurrent		= dlsym(lib, "glXMakeContextCurrent");

	glX.CreateContextAttribs	= glX.GetProcAddress("glXCreateContextAttribsARB");

	memset(&gl, 0x0, sizeof(gl_ptrs));

	gl.DebugMessageControl	= (PFNGLDEBUGMESSAGECONTROLPROC)glX.GetProcAddress("glDebugMessageControl");
	gl.DebugMessageInsert	= (PFNGLDEBUGMESSAGEINSERTPROC)glX.GetProcAddress("glDebugMessageInsert");
	gl.DebugMessageCallback	= (PFNGLDEBUGMESSAGECALLBACKPROC)glX.GetProcAddress("glDebugMessageCallback");
	gl.GetDebugMessageLog	= (PFNGLGETDEBUGMESSAGELOGPROC)glX.GetProcAddress("glGetDebugMessageLog");
	gl.PushDebugGroup		= (PFNGLPUSHDEBUGGROUPPROC)glX.GetProcAddress("glPushDebugGroup");
	gl.PopDebugGroup		= (PFNGLPOPDEBUGGROUPPROC)glX.GetProcAddress("glPopDebugGroup");

	glBreak_enable();
}

/**
 * @brief glBreak_release
 */
void glBreak_release(void)
{
	dlclose(lib);
}

/**
 * @brief glBreak_enable
 */
void glBreak_enable(void)
{
	if (NULL != gl.DebugMessageCallback && NULL != gl.DebugMessageControl)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		GLuint unusedIds = 0;

		gl.DebugMessageCallback(&glBreak_debug, NULL);
		gl.DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
	}
}

static void glBreak_print_source(GLenum source)
{
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
		{
			fputs("GL_DEBUG_SOURCE_API", stderr);
		}
		break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		{
			fputs("GL_DEBUG_SOURCE_WINDOW_SYSTEM", stderr);
		}
		break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
		{
			fputs("GL_DEBUG_SOURCE_SHADER_COMPILER", stderr);
		}
		break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
		{
			fputs("GL_DEBUG_SOURCE_THIRD_PARTY", stderr);
		}
		break;

		case GL_DEBUG_SOURCE_APPLICATION:
		{
			fputs("GL_DEBUG_SOURCE_APPLICATION", stderr);
		}
		break;

		case GL_DEBUG_SOURCE_OTHER:
		{
			fputs("GL_DEBUG_SOURCE_APPLICATION", stderr);
		}
		break;
	}
}

static void glBreak_print_type(GLenum type)
{
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
		{
			fputs("GL_DEBUG_TYPE_ERROR", stderr);
		}
		break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			fputs("GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR", stderr);
		}
		break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			fputs("GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR", stderr);
		}
		break;

		case GL_DEBUG_TYPE_PORTABILITY:
		{
			fputs("GL_DEBUG_TYPE_PORTABILITY", stderr);
		}
		break;

		case GL_DEBUG_TYPE_PERFORMANCE:
		{
			fputs("GL_DEBUG_TYPE_PERFORMANCE", stderr);
		}
		break;

		case GL_DEBUG_TYPE_MARKER:
		{
			fputs("GL_DEBUG_TYPE_MARKER", stderr);
		}
		break;

		case GL_DEBUG_TYPE_PUSH_GROUP:
		{
			fputs("GL_DEBUG_TYPE_PUSH_GROUP", stderr);
		}
		break;

		case GL_DEBUG_TYPE_POP_GROUP:
		{
			fputs("GL_DEBUG_TYPE_POP_GROUP", stderr);
		}
		break;

		case GL_DEBUG_TYPE_OTHER:
		{
			fputs("GL_DEBUG_TYPE_OTHER", stderr);
		}
		break;
	}
}

static void glBreak_print_severity(GLenum severity)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_LOW:
		{
			fputs("GL_DEBUG_SEVERITY_LOW", stderr);
		}
		break;

		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			fputs("GL_DEBUG_SEVERITY_MEDIUM", stderr);
		}
		break;

		case GL_DEBUG_SEVERITY_HIGH:
		{
			fputs("GL_DEBUG_SEVERITY_HIGH", stderr);
		}
		break;
	}
}

/**
 * @brief glBreak_debug
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param message
 * @param userParam
 */
void glBreak_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
{
	fputs("---- GL BREAKPOINT ----\n", stderr);

	fputs("Source : ", stderr);
	glBreak_print_source(source);

	fputs("\n", stderr);

	fputs("Type : ", stderr);
	glBreak_print_type(type);

	fputs("\n", stderr);

	fputs("Severity : ", stderr);
	glBreak_print_severity(severity);

	fputs("\n", stderr);

	fputs("Message : ", stderr);
	fputs(message, stderr);

	if (GL_DEBUG_TYPE_ERROR == type)
	{
		asm("int3");
	}

	fputs("-----------------------\n", stderr);
}

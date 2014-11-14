#pragma once

#include <GL/glcorearb.h>
#include <GL/glx.h>

typedef struct
{
	PFNGLDEBUGMESSAGECONTROLPROC	DebugMessageControl;
	PFNGLDEBUGMESSAGEINSERTPROC		DebugMessageInsert;
	PFNGLDEBUGMESSAGECALLBACKPROC	DebugMessageCallback;
	PFNGLGETDEBUGMESSAGELOGPROC		GetDebugMessageLog;
	PFNGLPUSHDEBUGGROUPPROC			PushDebugGroup;
	PFNGLPOPDEBUGGROUPPROC			PopDebugGroup;
} gl_ptrs;

extern gl_ptrs gl;

typedef void(*pfn)();

#ifndef PFNGLXCREATECONTEXTPROC
typedef GLXContext ( *PFNGLXCREATECONTEXTPROC) (Display *dpy, XVisualInfo * vis, GLXContext shareList, Bool direct);
#endif // PFNGLXCREATECONTEXTPROC

#ifndef PFNGLXMAKECURRENTPROC
typedef Bool ( *PFNGLXMAKECURRENTPROC) (Display *dpy, GLXDrawable drawable, GLXContext ctx);
#endif // PFNGLXMAKECURRENTPROC

typedef struct
{
	PFNGLXGETPROCADDRESSPROC GetProcAddress;
	PFNGLXMAKECURRENTPROC MakeCurrent;
	PFNGLXMAKECONTEXTCURRENTPROC MakeContextCurrent;
	PFNGLXCREATECONTEXTPROC CreateContext;
	PFNGLXCREATECONTEXTATTRIBSARBPROC CreateContextAttribs;
} glX_ptrs;

extern glX_ptrs glX;

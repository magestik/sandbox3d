#include "structs.h"
#include "functions.h"

#include <stdlib.h>
#include <string.h>

void glBreak_enable(void);

glX_ptrs glX;

static int * addDebugBit(const int * attrib_list)
{
	int count = 1; // start at 1 because the loop won't count None
	int * attrib = attrib_list;

	while (*attrib != None)
	{
		++attrib;
		++count;
	}

	int count2 = count + 2;

	int * attrib_list_debug = (int*)malloc(sizeof(int)*count2);
	memcpy(attrib_list_debug, attrib_list, sizeof(int)*count);

	attrib_list_debug[count2-3] = GLX_CONTEXT_FLAGS_ARB;
	attrib_list_debug[count2-2] = GLX_CONTEXT_DEBUG_BIT_ARB;
	attrib_list_debug[count2-1] = None;

	return(attrib_list_debug);
}

/**
 * @brief glXCreateContext
 * @param dpy
 * @param vis
 * @param shareList
 * @param direct
 * @return
 */
GLXContext glXCreateContext(Display * dpy, XVisualInfo * vis, GLXContext shareList, Bool direct)
{
	return(glX.CreateContext(dpy, vis, shareList, direct));
}

/**
 * @brief glXCreateContextAttribs
 * @param dpy
 * @param vis
 * @param shareList
 * @param direct
 * @return
 */
GLXContext glXCreateContextAttribs(Display * dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int * attrib_list)
{
	int * attrib_list_debug = addDebugBit(attrib_list);
	GLXContext ctx = glX.CreateContextAttribs(dpy, config, share_context, direct, attrib_list_debug);
	free(attrib_list_debug);
	return(ctx);
}


/**
 * @brief glXMakeCurrent
 * @param dpy
 * @param drawable
 * @param ctx
 * @return
 */
Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx)
{
	Bool r = glX.MakeCurrent(dpy, drawable, ctx);
	glBreak_enable();
	return(r);
}

/**
 * @brief glXMakeContextCurrent
 * @param display
 * @param draw
 * @param read
 * @param ctx
 * @return
 */
Bool glXMakeContextCurrent(Display * dpy,  GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
	Bool r = glX.MakeContextCurrent(dpy, draw, read, ctx);
	glBreak_enable();
	return(r);
}

/**
 * @brief glXGetProcAddress
 * @param procName
 * @return
 */
pfn glXGetProcAddress(const GLubyte * procName)
{
	if (!strcmp(procName, "glXCreateContextAttribs") || !strcmp(procName, "glXCreateContextAttribsARB"))
	{
		return((void*)&glXCreateContextAttribs);
	}
	else if (!strcmp(procName, "glDebugMessageControl") || !strcmp(procName, "glDebugMessageControlARB"))
	{
		return((void*)&glDebugMessageControl);
	}
	else if (!strcmp(procName, "glDebugMessageInsert") || !strcmp(procName, "glDebugMessageInsertARB"))
	{
		return((void*)&glDebugMessageInsert);
	}
	else if (!strcmp(procName, "glDebugMessageCallback") || !strcmp(procName, "glDebugMessageCallbackARB"))
	{
		return((void*)&glDebugMessageCallback);
	}
	else if (!strcmp(procName, "glGetDebugMessageLog") || !strcmp(procName, "glGetDebugMessageLogARB"))
	{
		return((void*)&glGetDebugMessageLog);
	}
	else if (!strcmp(procName, "glPushDebugGroup") || !strcmp(procName, "glPushDebugGroupARB"))
	{
		return((void*)&glPushDebugGroup);
	}
	else if (!strcmp(procName, "glPopDebugGroup") || !strcmp(procName, "glPopDebugGroupARB"))
	{
		return((void*)&glPopDebugGroup);
	}

	return(glX.GetProcAddress(procName));
}

/**
 * @brief glXGetProcAddressARB
 * @param procName
 * @return
 */
pfn glXGetProcAddressARB(const GLubyte * procName)
{
	return(glXGetProcAddress(procName));
}

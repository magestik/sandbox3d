#include "Rendering/Rendering.h"

//#include <RenderGraph.h>

#if defined _WIN32 || defined __CYGWIN__
#	ifdef Sandbox3D_EditorInterface_EXPORTS
#		ifdef __GNUC__
#			define DLL_PUBLIC __attribute__ ((dllexport))
#		else
#			define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#		endif
#	else
#		ifdef __GNUC__
#			define DLL_PUBLIC __attribute__ ((dllimport))
#		else
#			define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#		endif
#	endif
#	define DLL_LOCAL
#else
#	if __GNUC__ >= 4
#		define DLL_PUBLIC __attribute__ ((visibility ("default")))
#		define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#	else
#		define DLL_PUBLIC
#		define DLL_LOCAL
#	endif
#endif

namespace RenderGraph
{
	class Factory;
}

static Rendering  * g_pRendering = nullptr;

extern "C"
{

DLL_PUBLIC bool renderer_onLoad(void)
{
	return(true);
}

DLL_PUBLIC bool renderer_onInit(Scene & scene)
{
	g_pRendering = new Rendering(scene);
	return(g_pRendering != nullptr);
}

DLL_PUBLIC bool renderer_onRelease(void)
{
	delete g_pRendering;
	g_pRendering = nullptr;
	return(true);
}

DLL_PUBLIC void renderer_onReady(void)
{
	g_pRendering->onReady();
}

DLL_PUBLIC void renderer_onResize(unsigned int width, unsigned int height)
{
	g_pRendering->onResize(width, height);
}

DLL_PUBLIC void renderer_onUpdate(const mat4x4 & matView)
{
	g_pRendering->onUpdate(matView, vec4(0.7f, 0.7f, 0.7f, 1.0f));
}

DLL_PUBLIC void renderer_initQueue(const char * szFilename)
{
	g_pRendering->initQueue(szFilename);
}

DLL_PUBLIC void renderer_releaseQueue(void)
{
	g_pRendering->releaseQueue();
}

DLL_PUBLIC unsigned int renderer_getRenderTexture(const char * szName)
{
	const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = g_pRendering->GetRenderTexture(szName);

	if (nullptr != pRenderTexture)
	{
		return(pRenderTexture->GetObject());
	}

	return(0);
}

}

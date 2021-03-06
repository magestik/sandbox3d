#include "Rendering/Rendering.h"

#include "RenderGraph.h"

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

Rendering  * g_pRendering = nullptr;
static std::string g_strBaseDir;

extern "C"
{

DLL_PUBLIC bool renderer_onLoad(const char * baseDirectory)
{
	g_strBaseDir = baseDirectory;
	return(true);
}

DLL_PUBLIC bool renderer_onInit(Scene & scene, RenderGraph::Factory & factory)
{
	g_pRendering = new Rendering(scene, factory);
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
	g_pRendering->onReady(g_strBaseDir.data());
}

DLL_PUBLIC void renderer_onResize(unsigned int width, unsigned int height)
{
	g_pRendering->onResize(width, height);
}

DLL_PUBLIC void renderer_onUpdate(const mat4x4 & matView)
{
	g_pRendering->onUpdate(matView, vec4(0.7f, 0.7f, 0.7f, 1.0f));
}

}

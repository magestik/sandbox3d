#include "Rendering/Rendering.h"

//#include <RenderGraph.h>
namespace RenderGraph
{
	class Factory;
}

static Rendering  * g_pRendering = nullptr;

extern "C"
{

bool renderer_onLoad(void)
{
	return(true);
}

bool renderer_onInit(Scene & scene)
{
	g_pRendering = new Rendering(scene);
	return(g_pRendering != nullptr);
}

bool renderer_onRelease(void)
{
	delete g_pRendering;
	g_pRendering = nullptr;
	return(true);
}

void renderer_onReady(void)
{
	g_pRendering->onReady();
}

void renderer_onResize(unsigned int width, unsigned int height)
{
	g_pRendering->onResize(width, height);
}

void renderer_onUpdate(const mat4x4 & matView)
{
	g_pRendering->onUpdate(matView, vec4(0.7f, 0.7f, 0.7f, 1.0f));
}

void renderer_initQueue(const char * szFilename)
{
	g_pRendering->initQueue(szFilename);
}

void renderer_releaseQueue(void)
{
	g_pRendering->releaseQueue();
}

unsigned int renderer_getRenderTexture(const char * szName)
{
	const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = g_pRendering->GetRenderTexture(szName);

	if (nullptr != pRenderTexture)
	{
		return(pRenderTexture->GetObject());
	}

	return(0);
}

}

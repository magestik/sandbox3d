#pragma once

#include "RHI/RHI.h"

#include <Vector.h>
#include <Matrix.h>

#include <map>

class Rendering;
class GraphicsAlgorithm;

typedef GraphicsAlgorithm * (*GraphicsAlgorithmFactory)(Rendering & rendering, RHI::Framebuffer & framebuffer);

class GraphicsAlgorithm
{

public:

	GraphicsAlgorithm(Rendering & rendering, RHI::Framebuffer & framebuffer);
	~GraphicsAlgorithm();

	virtual bool init(void) = 0;
	virtual bool release(void) = 0;

	virtual bool render(RHI::CommandBuffer & commandBuffer) = 0;

	virtual void setParameter(const char * name, const char * value) = 0;

	static GraphicsAlgorithm * Create(const char * szType, Rendering & rendering, RHI::Framebuffer & framebuffer);
	static void RegisterEverything(void);

protected:

	Rendering & m_rendering;

	RHI::Framebuffer & m_framebuffer;

	RHI::RenderPass m_renderPass;

private:

	static std::map<std::string, GraphicsAlgorithmFactory> m_FactoryMap;
};


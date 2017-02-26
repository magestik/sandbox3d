#pragma once

#include "RHI/RHI.h"

#include <Vector.h>
#include <Matrix.h>

class Rendering;

class GraphicsAlgorithm
{

public:

	GraphicsAlgorithm(Rendering & rendering, RHI::Framebuffer & framebuffer);
	~GraphicsAlgorithm();

	virtual bool init(void) = 0;

	virtual bool render(RHI::CommandBuffer & commandBuffer) = 0;

protected:

	Rendering & m_rendering;

	RHI::Framebuffer & m_framebuffer;

	RHI::RenderPass m_renderPass;
};

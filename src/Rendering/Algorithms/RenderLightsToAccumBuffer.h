#pragma once

#include "../Rendering.h"

#define SHADOW_MAP_SIZE 1024

class RenderLightsToAccumBuffer : public GraphicsAlgorithm
{

public:

	RenderLightsToAccumBuffer(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~RenderLightsToAccumBuffer(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipelineDirectionalLight;
};

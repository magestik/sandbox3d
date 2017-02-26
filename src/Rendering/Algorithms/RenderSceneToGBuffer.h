#pragma once

#include "../Rendering.h"

class RenderSceneToGBuffer : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderSceneToGBuffer(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~RenderSceneToGBuffer(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipelineSimple;
	RHI::Pipeline m_pipelineNormalMap;
	RHI::Sampler m_sampler;
};

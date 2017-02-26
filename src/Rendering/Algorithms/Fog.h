#pragma once

#include "../Rendering.h"

class Fog : public GraphicsAlgorithm
{

public:

	Fog(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~Fog(void);


	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
	RHI::Sampler m_samplerDepthMap;
};

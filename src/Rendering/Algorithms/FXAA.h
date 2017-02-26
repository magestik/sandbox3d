#pragma once

#include "../Rendering.h"

class FXAA : public GraphicsAlgorithm
{

public:

	FXAA(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~FXAA(void);


	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
};

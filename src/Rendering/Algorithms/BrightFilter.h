#pragma once

#include "../Rendering.h"

class BrightFilter : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	BrightFilter(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~BrightFilter(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
};

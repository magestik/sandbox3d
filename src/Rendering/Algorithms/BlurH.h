#pragma once

#include "../Rendering.h"

class BlurH : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	explicit BlurH(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~BlurH(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
};

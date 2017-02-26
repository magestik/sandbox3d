#pragma once

#include "../Rendering.h"

class BrightFilter : public GraphicsAlgorithm
{

public:

	BrightFilter(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~BrightFilter(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

#pragma once

#include "../Rendering.h"

class BlurV : public GraphicsAlgorithm
{

public:

	BlurV(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~BlurV(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

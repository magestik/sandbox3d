#pragma once

#include "../Rendering.h"

class Compose : public GraphicsAlgorithm
{

public:

	Compose(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~Compose(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

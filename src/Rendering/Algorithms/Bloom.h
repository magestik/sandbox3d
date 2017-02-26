#pragma once

#include "../Rendering.h"

class Bloom : public GraphicsAlgorithm
{

public:

	Bloom(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~Bloom(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

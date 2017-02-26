#pragma once

#include "../Rendering.h"

class ToneMapping : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	explicit		ToneMapping		(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual			~ToneMapping	(void);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
};

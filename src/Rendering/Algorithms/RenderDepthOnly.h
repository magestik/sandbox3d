#pragma once

#include "../Rendering.h"

class RenderDepthOnly : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderDepthOnly(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~RenderDepthOnly(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual bool	render			(RHI::CommandBuffer & commandBuffer) override;
	virtual void	setParameter	(const char * name, const char * value) override;

private:

	RHI::Pipeline m_pipeline;
};

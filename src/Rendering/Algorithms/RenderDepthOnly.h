#pragma once

#include "../Rendering.h"

class RenderDepthOnly : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderDepthOnly();
	virtual ~RenderDepthOnly(void) override;

	static RenderGraph::Pass * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

#pragma once

#include "../Rendering.h"

class FXAA : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	FXAA(void);
	virtual ~FXAA(void) override;

	static RenderGraph::Operation * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
};

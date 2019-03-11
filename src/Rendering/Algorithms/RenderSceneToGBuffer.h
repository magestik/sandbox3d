#pragma once

#include "../Rendering.h"

class RenderSceneToGBuffer : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderSceneToGBuffer(void);
	virtual ~RenderSceneToGBuffer(void) override;

	static RenderGraph::Pass * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipelineSimple;
	RHI::Pipeline m_pipelineNormalMap;
	RHI::Sampler m_sampler;
};

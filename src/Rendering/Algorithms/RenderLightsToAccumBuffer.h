#pragma once

#include "../Rendering.h"

#define SHADOW_MAP_SIZE 1024

class RenderLightsToAccumBuffer : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderLightsToAccumBuffer();
	virtual ~RenderLightsToAccumBuffer(void) override;

	static RenderGraph::Operation * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipelineAmbientLight;
	RHI::Pipeline m_pipelineAmbientLightNoAO;
	RHI::Pipeline m_pipelineDirectionalLight;
	RHI::Sampler m_samplerDepth;
	RHI::Sampler m_samplerNormal;
};

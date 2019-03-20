#pragma once

#include "../Rendering.h"

class Compose : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	Compose();
	virtual ~Compose(void) override;

	static RenderGraph::Operation * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline_diffuse_specular;
	RHI::Pipeline m_pipeline_diffuse_only;
	RHI::Pipeline m_pipeline_specular_only;
	RHI::Pipeline m_pipeline_none;

	RHI::Sampler m_samplerDiffuseSampler;
	RHI::Sampler m_samplerSpecularSampler;
	RHI::Sampler m_samplerDiffuseLightSampler;
	RHI::Sampler m_samplerSpecularLightSampler;
	RHI::Sampler m_samplerShadowMap;
};

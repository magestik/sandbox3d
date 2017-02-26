#pragma once

#include "../Rendering.h"

class Compose : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	Compose(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~Compose(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_samplerDiffuseSampler;
	RHI::Sampler m_samplerSpecularSampler;
	RHI::Sampler m_samplerDiffuseLightSampler;
	RHI::Sampler m_samplerSpecularLightSampler;
	RHI::Sampler m_samplerShadowMap;
};

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
	virtual bool	init			(void) override;
	virtual bool	release			(void) override;
	virtual bool	render			(RHI::CommandBuffer & commandBuffer) override;
	virtual void	setParameter	(const char * name, const char * value) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_samplerDiffuseSampler;
	RHI::Sampler m_samplerSpecularSampler;
	RHI::Sampler m_samplerDiffuseLightSampler;
	RHI::Sampler m_samplerSpecularLightSampler;
	RHI::Sampler m_samplerShadowMap;

	const GPU::Texture<GL_TEXTURE_2D> * m_pDiffuseLightsTexture;
	const GPU::Texture<GL_TEXTURE_2D> * m_pSpecularLightsTexture;
};

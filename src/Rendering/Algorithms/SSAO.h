#pragma once

#include "../Rendering.h"

class SSAO : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	SSAO(void);
	virtual ~SSAO(void) override;

	static RenderGraph::Operation * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
	RHI::Sampler m_samplerNoise;

	GPU::Texture<GL_TEXTURE_2D> * m_pNoiseTexture;

	std::vector<vec3> m_kernel;
};

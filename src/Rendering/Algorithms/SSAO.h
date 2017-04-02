#pragma once

#include "../Rendering.h"

class SSAO : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	SSAO(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~SSAO(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual bool	release			(void) override;
	virtual bool	render			(RHI::CommandBuffer & commandBuffer) override;
	virtual void	setParameter	(const char * name, const char * value) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;
	RHI::Sampler m_samplerNoise;

	const GPU::Texture<GL_TEXTURE_2D> * m_pTextureDepth;
	const GPU::Texture<GL_TEXTURE_2D> * m_pTextureNormals;

	GPU::Texture<GL_TEXTURE_2D> m_noiseTexture;

	std::vector<vec3> m_kernel;
};

#pragma once

#include "../Rendering.h"

#define SHADOW_MAP_SIZE 1024

class RenderLightsToAccumBuffer : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	RenderLightsToAccumBuffer(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~RenderLightsToAccumBuffer(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual bool	release			(void) override;
	virtual bool	render			(RHI::CommandBuffer & commandBuffer) override;
	virtual void	setParameter	(const char * name, const char * value) override;

private:

	RHI::Pipeline m_pipelineAmbientLight;
	RHI::Pipeline m_pipelineDirectionalLight;
	RHI::Sampler m_samplerDepth;
	RHI::Sampler m_samplerNormal;

	const GPU::Texture<GL_TEXTURE_2D> * m_pDepthTexture;
	const GPU::Texture<GL_TEXTURE_2D> * m_pNormalsTexture;

	const GPU::Texture<GL_TEXTURE_2D> * m_pAmbientOcclusionTexture;

};

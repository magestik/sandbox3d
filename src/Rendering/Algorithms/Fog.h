#pragma once

#include "../Rendering.h"

class Fog : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	Fog(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~Fog(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual bool	release			(void) override;
	virtual bool	render			(RHI::CommandBuffer & commandBuffer) override;
	virtual void	setParameter	(const char * name, const char * value) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_samplerDepthMap;

	const GPU::Texture<GL_TEXTURE_2D> * m_pDepthTexture;
};

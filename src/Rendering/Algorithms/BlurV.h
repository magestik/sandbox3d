#pragma once

#include "../Rendering.h"

class BlurV : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	BlurV(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual ~BlurV(void);

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

	const GPU::Texture<GL_TEXTURE_2D> * m_pTexture;
};

#pragma once

#include "../Rendering.h"

#define SHADOW_MAP_SIZE 1024

class RenderSceneToShadowMap : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	explicit		RenderSceneToShadowMap	(Rendering & rendering, RHI::Framebuffer & framebuffer);
	virtual			~RenderSceneToShadowMap	(void);

	static GraphicsAlgorithm * Create(Rendering & rendering, RHI::Framebuffer & framebuffer);

	//
	// override GraphicsAlgorithm
	virtual bool	init		(void) override;
	virtual bool	render		(RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

#pragma once

#include "../Rendering.h"

#define SHADOW_MAP_SIZE 1024

class RenderSceneToShadowMap : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	explicit		RenderSceneToShadowMap	(void);
	virtual			~RenderSceneToShadowMap	(void) override;

	static RenderGraph::Pass * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
};

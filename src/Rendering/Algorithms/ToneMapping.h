#pragma once

#include "../Rendering.h"

class ToneMapping : public GraphicsAlgorithm
{

public:

	//
	// Constructor / Destructor
	explicit		ToneMapping		(void);
	virtual			~ToneMapping	(void) override;

	static RenderGraph::Pass * Create();

	//
	// override GraphicsAlgorithm
	virtual bool	init			(void) override;
	virtual void	release			(void) override;
	virtual bool	render			(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) override;

private:

	RHI::Pipeline m_pipeline;
	RHI::Sampler m_sampler;

	float m_fAverageLuminance;
	float m_fWhite2;
};

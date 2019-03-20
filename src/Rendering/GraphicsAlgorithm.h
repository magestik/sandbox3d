#pragma once

#include "RHI/RHI.h"

#include <Vector.h>
#include <Matrix.h>

#include <map>

#include "RenderGraph.h"

class Rendering;
class GraphicsAlgorithm;

typedef GraphicsAlgorithm * (*GraphicsAlgorithmFactory)(void);

class GraphicsAlgorithm : public RenderGraph::Pass
{

public:

	GraphicsAlgorithm();
	~GraphicsAlgorithm() override;

	virtual bool init(void) override = 0;
	virtual void release(void) override = 0;

	virtual bool render(RenderGraph::Parameters & parameters) override;

protected:

	virtual bool render(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer) = 0;

protected:

	Rendering & m_rendering;

	RHI::Framebuffer m_framebuffer;

	RHI::RenderPass m_renderPass;

private:

	static std::map<std::string, GraphicsAlgorithmFactory> m_FactoryMap;
};

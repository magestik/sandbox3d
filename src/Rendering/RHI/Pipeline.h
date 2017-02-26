#pragma once

#include <vector>

namespace RHI
{

class Pipeline
{
public:

	friend class CommandBuffer;


	//
	// Constructors / Destructor
	Pipeline		(void);
	Pipeline		(const PipelineInputAssemblyStateCreateInfo & input, const PipelineRasterizationStateCreateInfo & rasterization, const PipelineDepthStencilStateCreateInfo & depthStencil, const PipelineBlendStateCreateInfo & blend, const std::vector<PipelineShaderStageCreateInfo> & aStages);
	~Pipeline		(void);

public:

#if HAVE_OPENGL
	PipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
	PipelineRasterizationStateCreateInfo m_rasterizationState;
	PipelineDepthStencilStateCreateInfo m_depthStencilState;
	PipelineBlendStateCreateInfo m_blendState;
	GLuint m_uShaderObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkPipeline m_pipeline;
#endif // HAVE_VULKAN
};

}

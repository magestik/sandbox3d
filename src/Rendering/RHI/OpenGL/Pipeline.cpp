#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief Default constructor
 */
RHI::Pipeline::Pipeline(void)
: m_inputAssemblyState()
, m_depthStencilState()
, m_blendState()
{
	// ...
}

/**
 * @brief Constructor
 * @param input
 */
RHI::Pipeline::Pipeline(const PipelineInputAssemblyStateCreateInfo & input, const PipelineRasterizationStateCreateInfo & rasterization, const PipelineDepthStencilStateCreateInfo & depthStencil, const PipelineBlendStateCreateInfo & blend, const std::vector<PipelineShaderStageCreateInfo> & aStages)
: m_inputAssemblyState(input)
, m_rasterizationState(rasterization)
, m_depthStencilState(depthStencil)
, m_blendState(blend)
{
	m_uShaderObject = glCreateProgram();

	for (PipelineShaderStageCreateInfo stage : aStages)
	{
		glAttachShader(m_uShaderObject, stage.module);
	}

	glLinkProgram(m_uShaderObject);

	int status = 0;
	glGetProgramiv(m_uShaderObject, GL_LINK_STATUS, &status);

	assert(GL_TRUE == status);
}

/**
 * @brief Destructor
 */
RHI::Pipeline::~Pipeline(void)
{
	// ...
}

#endif // HAVE_OPENGL

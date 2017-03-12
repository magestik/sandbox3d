#include "CelShading.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
CelShading::CelShading(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pEdgesTexture(nullptr), m_pColorTexture(nullptr), m_fThreshold(0.5f)
{
	// ...
}

/**
 * @brief Destructor
 */
CelShading::~CelShading(void)
{
	// ...
}

/**
 * @brief CelShading::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * CelShading::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new CelShading(rendering, framebuffer));
}

/**
 * @brief CelShading::init
 * @return
 */
bool CelShading::init(void)
{
	//
	// Initialize Render Pass
	{
		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0; // disable depth buffer
		desc.aColorAttachments.push_back(0);

		m_renderPass = RHI::RenderPass(desc);
	}

	//
	// Initialize Pipelines
	{
		RHI::PipelineInputAssemblyStateCreateInfo input;
		RHI::PipelineRasterizationStateCreateInfo rasterization;
		RHI::PipelineDepthStencilStateCreateInfo depthStencil;
		RHI::PipelineBlendStateCreateInfo blend;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = m_rendering.m_mapShaderModules["fullscreen.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["cel_shading.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo sampler;
		sampler.minFilter = RHI::FILTER_NEAREST;
		sampler.magFilter = RHI::FILTER_NEAREST;
		sampler.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_sampler = RHI::Sampler(sampler);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief CelShading::render
 * @param commandBuffer
 * @return
 */
bool CelShading::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(CelShading);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "edgeSampler", 0, *m_pEdgesTexture, m_sampler);
		SetTexture(m_pipeline.m_uShaderObject, "colorSampler", 1, *m_pColorTexture, m_sampler);
		SetUniform(m_pipeline.m_uShaderObject, "threshold", m_fThreshold);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief CelShading::setParameter
 * @param name
 * @param value
 */
void CelShading::setParameter(const char * name, const char * value)
{
	if (!strcmp("edges", name))
	{
		m_pEdgesTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else if (!strcmp("color", name))
	{
		m_pColorTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else if (!strcmp("threshold", name))
	{
		m_fThreshold = atof(value);
	}
	else
	{
		assert(false);
	}
}

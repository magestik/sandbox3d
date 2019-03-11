#include "CelShading.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
CelShading::CelShading() : GraphicsAlgorithm(), m_fThreshold(0.5f)
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
RenderGraph::Pass * CelShading::Create()
{
	return(new CelShading());
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
 * @brief CelShading::release
 * @return
 */
void CelShading::release(void)
{
	// TODO
}

/**
 * @brief CelShading::render
 * @param commandBuffer
 * @return
 */
bool CelShading::render(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	assert(parameters.size() == 2);

	rmt_ScopedOpenGLSample(CelShading);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "edgeSampler", 0, parameters[0], m_sampler);
		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "colorSampler", 1, parameters[1], m_sampler);
		SetUniform(m_pipeline.m_uShaderObject, "threshold", m_fThreshold);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

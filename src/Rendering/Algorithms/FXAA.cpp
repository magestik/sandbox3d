#include "FXAA.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
FXAA::FXAA(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pTexture(nullptr)
{
	// ...
}

/**
 * @brief Destructor
 */
FXAA::~FXAA(void)
{
	// ...
}

/**
 * @brief FXAA::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * FXAA::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new FXAA(rendering, framebuffer));
}

/**
 * @brief FXAA::init
 * @return
 */
bool FXAA::init(void)
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
		fragmentShader.module = m_rendering.m_mapShaderModules["fullscreen_fxaa.frag"];

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
 * @brief FXAA::release
 * @return
 */
bool FXAA::release(void)
{
	return(false); // TODO
}

/**
 * @brief FXAA::render
 * @param commandBuffer
 * @return
 */
bool FXAA::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(FXAA);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *m_pTexture, m_sampler);

		SetUniform(m_pipeline.m_uShaderObject, "fxaaQualityRcpFrame", vec2(1.0/m_rendering.GetWidth(), 1.0/m_rendering.GetHeight()));

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief FXAA::setParameter
 * @param name
 * @param value
 */
void FXAA::setParameter(const char * name, const char * value)
{
	if (!strcmp("texture", name))
	{
		m_pTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else
	{
		assert(false);
	}
}

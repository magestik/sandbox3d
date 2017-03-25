#include "Bloom.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
Bloom::Bloom(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pTexture(nullptr)
{
	// ...
}

/**
 * @brief Destructor
 */
Bloom::~Bloom(void)
{
	// ...
}

/**
 * @brief Bloom::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * Bloom::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new Bloom(rendering, framebuffer));
}

/**
 * @brief Bloom::init
 * @return
 */
bool Bloom::init(void)
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
		blend.enable = true;
		blend.srcColorFactor = RHI::BLEND_FACTOR_ONE;
		blend.dstColorFactor = RHI::BLEND_FACTOR_ONE;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = m_rendering.m_mapShaderModules["fullscreen.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["fullscreen_color.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo sampler;
		sampler.minFilter = RHI::FILTER_LINEAR;
		sampler.magFilter = RHI::FILTER_LINEAR;
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
 * @brief Bloom::release
 * @return
 */
bool Bloom::release(void)
{
	return(false); // TODO
}

/**
 * @brief Bloom::render
 * @param commandBuffer
 * @return
 */
bool Bloom::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(Bloom);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *m_pTexture, m_sampler);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief Bloom::setParameter
 * @param name
 * @param value
 */
void Bloom::setParameter(const char * name, const char * value)
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

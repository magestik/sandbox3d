#include "BrightFilter.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
BrightFilter::BrightFilter(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pTexture(nullptr)
{
	// ...
}

/**
 * @brief Destructor
 */
BrightFilter::~BrightFilter(void)
{
	// ...
}

/**
 * @brief BrightFilter::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * BrightFilter::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new BrightFilter(rendering, framebuffer));
}

/**
 * @brief BrightFilter::init
 * @return
 */
bool BrightFilter::init(void)
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
		fragmentShader.module = m_rendering.m_mapShaderModules["fullscreen_bright.frag"];

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
 * @brief BrightFilter::release
 * @return
 */
bool BrightFilter::release(void)
{
	return(false); // TODO
}

/**
 * @brief BrightFilter::render
 * @param commandBuffer
 * @return
 */
bool BrightFilter::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(BrightFilter);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth()/4, m_rendering.GetHeight()/4));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *m_pTexture, m_sampler);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief BrightFilter::setParameter
 * @param name
 * @param value
 */
void BrightFilter::setParameter(const char * name, const char * value)
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

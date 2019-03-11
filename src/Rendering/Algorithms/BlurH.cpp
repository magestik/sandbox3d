#include "BlurH.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
BlurH::BlurH() : GraphicsAlgorithm()
{
	// ...
}

/**
 * @brief Destructor
 */
BlurH::~BlurH(void)
{
	// ...
}

/**
 * @brief BlurH::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
RenderGraph::Pass * BlurH::Create()
{
	return(new BlurH());
}

/**
 * @brief BlurH::init
 * @return
 */
bool BlurH::init(void)
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
		fragmentShader.module = m_rendering.m_mapShaderModules["gaussian_blur_h.frag"];

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
 * @brief BlurH::release
 * @return
 */
void BlurH::release(void)
{
	// TODO
}

/**
 * @brief BlurH::render
 * @param commandBuffer
 * @return
 */
bool BlurH::render(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(BlurH);

	if (parameters.size() != 1)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	assert(parameters[0].first == 0);
	const GLuint inputTexture = parameters[0].second.asUInt;

	ivec2 viewport(m_rendering.GetWidth(), m_rendering.GetHeight()); // FIXME : m_pTexture H/W

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), viewport);
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "texSampler", 0, inputTexture, m_sampler);

		m_rendering.m_pQuadMesh->draw(commandBuffer);

	}
	commandBuffer.EndRenderPass();

	return(true);
}

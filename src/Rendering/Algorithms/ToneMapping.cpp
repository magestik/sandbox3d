#include "ToneMapping.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
ToneMapping::ToneMapping() : GraphicsAlgorithm()
{
	// ...
}

/**
 * @brief Destructor
 */
ToneMapping::~ToneMapping(void)
{
	// ...
}

/**
 * @brief ToneMapping::Create
 * @return
 */
RenderGraph::Operation * ToneMapping::Create()
{
	return new ToneMapping();
}

/**
 * @brief ToneMapping::init
 * @return
 */
bool ToneMapping::init(void)
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
		fragmentShader.module = m_rendering.m_mapShaderModules["tonemapping.frag"];

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
 * @brief ToneMapping::release
 * @return
 */
void ToneMapping::release(void)
{
	// TODO
}

/**
 * @brief ToneMapping::render
 * @param commandBuffer
 * @return
 */
bool ToneMapping::render(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(ToneMapping);

	if (parameters.size() < 3)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	const float white2 = parameters.pop().asFloat;
	const float avLum = parameters.pop().asFloat;
	const GLuint inputTexture = parameters.pop().asUInt;

	if (inputTexture == 0)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "texSampler", 0, inputTexture, m_sampler);

		SetUniform(m_pipeline.m_uShaderObject, "avLum", avLum);
		SetUniform(m_pipeline.m_uShaderObject, "white2", white2);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	{
		GLint texture = 0;
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);

		RenderGraph::Value v;
		v.asUInt = texture;
		parameters.push(v);
	}

	return true;
}

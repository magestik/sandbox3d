#include "SSAO.h"

#include "Remotery.h"

#include <random>

/**
 * @brief Constructor
 * @param rendering
 */
SSAO::SSAO() : GraphicsAlgorithm()
{
	// ...
}

/**
 * @brief Destructor
 */
SSAO::~SSAO(void)
{
	// ...
}

/**
 * @brief SSAO::Create
 * @return
 */
RenderGraph::Operation * SSAO::Create()
{
	return new SSAO();
}

/**
 * @brief SSAO::init
 * @return
 */
bool SSAO::init(void)
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
		vertexShader.module = m_rendering.m_mapShaderModules["fullscreen_viewray.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["fullscreen_ssao.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo sampler1;
		sampler1.minFilter = RHI::FILTER_NEAREST;
		sampler1.magFilter = RHI::FILTER_NEAREST;
		sampler1.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler1.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_sampler = RHI::Sampler(sampler1);

		RHI::SamplerCreateInfo sampler2;
		sampler2.minFilter = RHI::FILTER_NEAREST;
		sampler2.magFilter = RHI::FILTER_NEAREST;
		sampler2.addressModeU = RHI::SAMPLER_ADDRESS_MODE_REPEAT;
		sampler2.addressModeV = RHI::SAMPLER_ADDRESS_MODE_REPEAT;

		m_samplerNoise = RHI::Sampler(sampler2);
	}

	//
	// Generate Kernel
	{
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
		std::default_random_engine generator;

		for (GLuint i = 0; i < 64; ++i)
		{
			vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
			);
			sample = normalize(sample);
			sample = sample * randomFloats(generator);
			float scale = GLfloat(i) / 64.0;
			scale = mix(0.1f, 1.0f, scale * scale);
			sample = sample * scale;
			m_kernel.push_back(sample);
		}

		std::vector<vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++)
		{
			vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
			ssaoNoise.push_back(noise);
		}

		m_pNoiseTexture = new GPU::Texture<GL_TEXTURE_2D>();
		m_pNoiseTexture->init<GL_RGB32F>(4, 4, (void*)ssaoNoise.data(), GL_RGB, GL_FLOAT);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

//	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlockFrag", Rendering::BLOCK_BINDING_CAMERA);

	return(true);
}

/**
 * @brief SSAO::release
 * @return
 */
void SSAO::release(void)
{
	m_kernel.clear();

	// TODO
}

/**
 * @brief SSAO::render
 * @param commandBuffer
 * @return
 */
bool SSAO::render(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(SSAO);

	if (parameters.size() < 3)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	const float radius = parameters.pop().asFloat;
	const GLuint inputTextureNormal = parameters.pop().asUInt;
	const GLuint inputTextureDepth = parameters.pop().asUInt;

	if (inputTextureDepth == 0 || inputTextureNormal == 0)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "depthSampler", 0, inputTextureDepth, m_sampler);
		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "normalSampler", 1, inputTextureNormal, m_sampler);

		SetTexture(m_pipeline.m_uShaderObject, "noiseSampler", 2, *m_pNoiseTexture, m_samplerNoise);

		SetUniform(m_pipeline.m_uShaderObject, "samples", m_kernel.data(), m_kernel.size());
		SetUniform(m_pipeline.m_uShaderObject, "noiseScale", vec2(m_rendering.GetWidth()/4.0f, m_rendering.GetHeight()/4.0f));
		SetUniform(m_pipeline.m_uShaderObject, "radius", radius);

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

	return(true);
}

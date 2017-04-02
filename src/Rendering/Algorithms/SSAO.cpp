#include "SSAO.h"

#include "Remotery.h"

#include <random>

/**
 * @brief Constructor
 * @param rendering
 */
SSAO::SSAO(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pTextureDepth(nullptr), m_pTextureNormals(nullptr)
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
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * SSAO::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new SSAO(rendering, framebuffer));
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

		m_noiseTexture.init<GL_RGB32F>(4, 4, (void*)ssaoNoise.data(), GL_RGB, GL_FLOAT);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlockFrag", Rendering::BLOCK_BINDING_CAMERA);

	return(true);
}

/**
 * @brief SSAO::release
 * @return
 */
bool SSAO::release(void)
{
	m_kernel.clear();

	return(false); // TODO
}

/**
 * @brief SSAO::render
 * @param commandBuffer
 * @return
 */
bool SSAO::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(SSAO);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "depthSampler", 0, *m_pTextureDepth, m_sampler);
		SetTexture(m_pipeline.m_uShaderObject, "normalSampler", 1, *m_pTextureNormals, m_sampler);
		SetTexture(m_pipeline.m_uShaderObject, "noiseSampler", 2, m_noiseTexture, m_samplerNoise);

		SetUniform(m_pipeline.m_uShaderObject, "noiseScale", vec2(m_rendering.GetWidth()/4.0f, m_rendering.GetHeight()/4.0f));
		SetUniform(m_pipeline.m_uShaderObject, "samples", m_kernel.data(), m_kernel.size());

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief SSAO::setParameter
 * @param name
 * @param value
 */
void SSAO::setParameter(const char * name, const char * value)
{
	if (!strcmp("geometry_depth", name))
	{
		m_pTextureDepth = m_rendering.m_mapTargets[value].getTexture();
	}
	else if (!strcmp("geometry_normals", name))
	{
		m_pTextureNormals = m_rendering.m_mapTargets[value].getTexture();
	}
	else
	{
		assert(false);
	}
}

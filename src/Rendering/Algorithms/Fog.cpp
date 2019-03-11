#include "Fog.h"

#include "../utils.inl"

#include "Remotery.h"

// dst = src.a * dst + 1 * src.rgb
// src.a : Extinction ; src.rgb = Scattering

/**
 * @brief Constructor
 * @param rendering
 */
Fog::Fog() : GraphicsAlgorithm()
{
	// ...
}

/**
 * @brief Destructor
 */
Fog::~Fog(void)
{
	// ...
}

/**
 * @brief Fog::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
RenderGraph::Pass * Fog::Create()
{
	return(new Fog());
}

/**
 * @brief RenderFogAlgorithm::init
 * @return
 */
bool Fog::init(void)
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
		blend.colorEquation = RHI::BLEND_OP_ADD;
		blend.srcColorFactor = RHI::BLEND_FACTOR_ONE;
		blend.dstColorFactor = RHI::BLEND_FACTOR_SRC_ALPHA;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = m_rendering.m_mapShaderModules["fullscreen.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["fog_simple.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo sampler2;
		sampler2.minFilter = RHI::FILTER_NEAREST;
		sampler2.magFilter = RHI::FILTER_NEAREST;
		sampler2.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler2.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_samplerDepthMap = RHI::Sampler(sampler2);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief Fog::release
 * @return
 */
void Fog::release(void)
{
	// TODO
}

/**
 * @brief Fog::render
 * @param commandBuffer
 * @return
 */
bool Fog::render(const RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(Fog);

	if (parameters.size() != 1)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return false;
	}

	assert(parameters[0].first == 0);
	const GLuint inputTexture = parameters[0].second.asUInt;

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));

	{
		commandBuffer.Bind(m_pipeline);

		SetTexture<GL_TEXTURE_2D>(m_pipeline.m_uShaderObject, "depthMapSampler", 0, inputTexture, m_samplerDepthMap);

		SetUniform(m_pipeline.m_uShaderObject, "FogScattering", m_rendering.GetScene().fog.Scattering);
		SetUniform(m_pipeline.m_uShaderObject, "FogExtinction", m_rendering.GetScene().fog.Extinction);
		SetUniform(m_pipeline.m_uShaderObject, "FogColor", m_rendering.GetScene().fog.Color);
		SetUniform(m_pipeline.m_uShaderObject, "camera_near", 1.0f);
		SetUniform(m_pipeline.m_uShaderObject, "camera_far", 1000.0f);
		SetUniform(m_pipeline.m_uShaderObject, "near_plane_half_size", vec2(m_rendering.GetHeight() * float(m_rendering.GetWidth()/(float)m_rendering.GetHeight()), tanf(75.0f/2.0)));

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}

	commandBuffer.EndRenderPass();

	return(true);
}

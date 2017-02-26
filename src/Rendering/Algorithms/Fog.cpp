#include "Fog.h"

#include "../utils.inl"

// dst = src.a * dst + 1 * src.rgb
// src.a : Extinction ; src.rgb = Scattering

/**
 * @brief Constructor
 * @param rendering
 */
Fog::Fog(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
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
GraphicsAlgorithm * Fog::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new Fog(rendering, framebuffer));
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
		RHI::SamplerCreateInfo sampler;
		sampler.minFilter = RHI::FILTER_NEAREST;
		sampler.magFilter = RHI::FILTER_NEAREST;
		sampler.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_sampler = RHI::Sampler(sampler);

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
 * @brief RenderFogAlgorithm::render
 * @param commandBuffer
 * @return
 */
bool Fog::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));

	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "depthMapSampler", 0, *(m_rendering.m_mapTargets["depth"].getTexture()), m_samplerDepthMap);

		SetUniform(m_pipeline.m_uShaderObject, "FogScattering", m_rendering.environment.fog.Scattering);
		SetUniform(m_pipeline.m_uShaderObject, "FogExtinction", m_rendering.environment.fog.Extinction);
		SetUniform(m_pipeline.m_uShaderObject, "FogColor", m_rendering.environment.fog.Color);
		SetUniform(m_pipeline.m_uShaderObject, "camera_near", 1.0f);
		SetUniform(m_pipeline.m_uShaderObject, "camera_far", 1000.0f);
		SetUniform(m_pipeline.m_uShaderObject, "near_plane_half_size", vec2(m_rendering.GetHeight() * float(m_rendering.GetWidth()/(float)m_rendering.GetHeight()), tanf(75.0f/2.0)));

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}

	commandBuffer.EndRenderPass();

	return(true);
}

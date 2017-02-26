#include "Fog.h"

#include "../utils.inl"

const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

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
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;
		RHI::Pipeline::DepthStencilState depthStencil;

		RHI::Pipeline::BlendState blend;
		blend.enable = true;
		blend.colorEquation = RHI::BLEND_OP_ADD;
		blend.srcColorFactor = RHI::BLEND_FACTOR_ONE;
		blend.dstColorFactor = RHI::BLEND_FACTOR_SRC_ALPHA;

		RHI::Pipeline::ShaderStage vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["fullscreen.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["fog_simple.frag"]->GetObject();

		std::vector<RHI::Pipeline::ShaderStage> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
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

		SetTexture(m_pipeline.m_uShaderObject, "depthMapSampler", 0, *(m_rendering.m_mapTargets["depth"].getTexture()), m_rendering.GetPipeline("fog_simple")->m_mapSamplers["depthMapSampler"]);

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
#include "FXAA.h"

const float avLum = 0.5f;
const float white2 = 0.5f;

/**
 * @brief Constructor
 * @param rendering
 */
FXAA::FXAA(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
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
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;
		RHI::Pipeline::DepthStencilState depthStencil;
		RHI::Pipeline::BlendState blend;

		RHI::Pipeline::ShaderStage vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["fullscreen.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["fullscreen_fxaa.frag"]->GetObject();

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
 * @brief FXAA::render
 * @param commandBuffer
 * @return
 */
bool FXAA::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *(m_rendering.m_mapTargets["LDR"].getTexture()), m_rendering.GetPipeline("fxaa")->m_mapSamplers["texSampler"]);

		SetUniform(m_pipeline.m_uShaderObject, "fxaaQualityRcpFrame", vec2(1.0/m_rendering.GetWidth(), 1.0/m_rendering.GetHeight()));

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

#include "ToneMapping.h"

const float avLum = 0.5f;
const float white2 = 0.5f;

/**
 * @brief Constructor
 * @param rendering
 */
ToneMapping::ToneMapping(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
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
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;
		RHI::Pipeline::DepthStencilState depthStencil;
		RHI::Pipeline::BlendState blend;

		RHI::Pipeline::ShaderStage vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["fullscreen.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["tonemapping.frag"]->GetObject();

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
 * @brief ToneMapping::render
 * @param commandBuffer
 * @return
 */
bool ToneMapping::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *(m_rendering.m_mapTargets["HDR"].getTexture()), m_rendering.GetPipeline("tonemapping")->m_mapSamplers["texSampler"]);

		SetUniform(m_pipeline.m_uShaderObject, "avLum", avLum);
		SetUniform(m_pipeline.m_uShaderObject, "white2", white2);

		m_rendering.m_pQuadMesh->draw(commandBuffer);
	}
	commandBuffer.EndRenderPass();

	return(true);
}

#include "BlurH.h"

/**
 * @brief Constructor
 * @param rendering
 */
BlurH::BlurH(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
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
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;
		RHI::Pipeline::DepthStencilState depthStencil;
		RHI::Pipeline::BlendState blend;

		RHI::Pipeline::ShaderStage vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["fullscreen.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["gaussian_blur_h.frag"]->GetObject();

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
 * @brief BlurH::render
 * @param commandBuffer
 * @return
 */
bool BlurH::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth()/4, m_rendering.GetHeight()/4));
	{
		commandBuffer.Bind(m_pipeline);

		SetTexture(m_pipeline.m_uShaderObject, "texSampler", 0, *(m_rendering.m_mapTargets["bloom1"].getTexture()), m_rendering.GetPipeline("bloom_horizontal_blur")->m_mapSamplers["texSampler"]);

		m_rendering.m_pQuadMesh->draw(commandBuffer);

	}
	commandBuffer.EndRenderPass();

	return(true);
}

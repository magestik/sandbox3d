#include "RenderSceneToShadowMap.h"

#include "../utils.inl"

/**
 * @brief Constructor
 * @param rendering
 */
RenderSceneToShadowMap::RenderSceneToShadowMap(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
RenderSceneToShadowMap::~RenderSceneToShadowMap(void)
{
	// ...
}

/**
 * @brief RenderSceneToShadowMap::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * RenderSceneToShadowMap::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new RenderSceneToShadowMap(rendering, framebuffer));
}

/**
 * @brief RenderSceneToShadowMap::init
 * @return
 */
bool RenderSceneToShadowMap::init(void)
{
	//
	// Initialize Render Pass
	{
		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0; // only write to depth buffer

		m_renderPass = RHI::RenderPass(desc);
	}

	//
	// Initialize Pipelines
	{
		RHI::PipelineInputAssemblyStateCreateInfo input;

		RHI::PipelineRasterizationStateCreateInfo rasterization;
		rasterization.enableDepthBias = true;
		rasterization.depthBiasConstantFactor = 10.0f;
		rasterization.depthBiasClamp = 0.0f;
		rasterization.depthBiasSlopeFactor = 1.0f;

		RHI::PipelineDepthStencilStateCreateInfo depthStencil;
		depthStencil.enableDepth = true;
		depthStencil.depthState.enableWrite = true;
		depthStencil.depthState.compareOp = RHI::COMPARE_OP_LESS;

		RHI::PipelineBlendStateCreateInfo blend;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["depth_only.vert"]->GetObject();

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["depth_only.frag"]->GetObject();

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
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
 * @brief RenderSceneToShadowMap::render
 * @param commandBuffer
 * @return
 */
bool RenderSceneToShadowMap::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE), 1.0f, 0);
	{
		commandBuffer.Bind(m_pipeline);

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_rendering.m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_rendering.m_matShadowMapProjection * mDepthView;

		SetUniform(m_pipeline.m_uShaderObject, "LightViewProjection", mDepthViewProjection);

		for (Mesh::Instance & object : m_rendering.m_aObjects)
		{
			SetUniform(m_pipeline.m_uShaderObject, "Model", object.transformation);

			object.mesh->bind();

			// TODO : remove loop and directly use glDrawElements on the full buffer
			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw(commandBuffer);
			}

			object.mesh->unbind();
		}
	}
	commandBuffer.EndRenderPass();

	return(true);
}

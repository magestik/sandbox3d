#include "RenderSceneToGBuffer.h"

/**
 * @brief Constructor
 * @param rendering
 */
RenderSceneToGBuffer::RenderSceneToGBuffer(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
RenderSceneToGBuffer::~RenderSceneToGBuffer(void)
{
	// ...
}

/**
 * @brief RenderSceneToGBufferAlgorithm::init
 * @return
 */
bool RenderSceneToGBuffer::init(void)
{
	//
	// Initialize Render Pass
	{
		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0; // depth buffer required for Z-test
		desc.aColorAttachments.push_back(0);

		m_renderPass = RHI::RenderPass(desc);
	}

	//
	// Initialize Pipelines
	{
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;

		RHI::Pipeline::DepthStencilState depthStencil;
		depthStencil.enableDepth = true;
		depthStencil.depthState.enableWrite = true;
		depthStencil.depthState.compareOp = RHI::COMPARE_OP_LESS;

		RHI::Pipeline::BlendState blend;

		RHI::Pipeline::ShaderStage vertexShaderSimple;
		vertexShaderSimple.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShaderSimple.module = g_VertexShaders["geometry_pass.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShaderSimple;
		fragmentShaderSimple.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShaderSimple.module = g_FragmentShaders["geometry_pass.frag"]->GetObject();

		std::vector<RHI::Pipeline::ShaderStage> aStagesSimple;
		aStagesSimple.push_back(vertexShaderSimple);
		aStagesSimple.push_back(fragmentShaderSimple);

		RHI::Pipeline::ShaderStage vertexShaderNM;
		vertexShaderNM.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShaderNM.module = g_VertexShaders["geometry_normalmap_pass.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShaderNM;
		fragmentShaderNM.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShaderNM.module = g_FragmentShaders["geometry_normalmap_pass.frag"]->GetObject();

		std::vector<RHI::Pipeline::ShaderStage> aStagesNM;
		aStagesNM.push_back(vertexShaderNM);
		aStagesNM.push_back(fragmentShaderNM);

		m_pipelineSimple = RHI::Pipeline(input, rasterization, depthStencil, blend, aStagesSimple);
		m_pipelineNormalMap = RHI::Pipeline(input, rasterization, depthStencil, blend, aStagesNM);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipelineSimple.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipelineSimple.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);
	SetUniformBlockBinding(m_pipelineNormalMap.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipelineNormalMap.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief RenderSceneToGBufferAlgorithm::render
 * @param commandBuffer
 * @return
 */
bool RenderSceneToGBuffer::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), vec4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);

	{

		// OPTIMIZE THIS !!!!!
		{
			commandBuffer.Bind(m_pipelineSimple);

			unsigned int offset = 0;

			for (Mesh::Instance & object : m_rendering.m_aObjects)
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

				object.mesh->bind();

				for (SubMesh * m : object.getDrawCommands())
				{
					const GPU::Texture<GL_TEXTURE_2D> * pNormalMap = m->getNormalMap();

					if (nullptr == pNormalMap)
					{
						SetUniform(m_pipelineSimple.m_uShaderObject, "shininess", m->m_material.shininess);

						m->draw(commandBuffer);
					}
				}

				object.mesh->unbind();

				++offset;
			}
		}

		{
			commandBuffer.Bind(m_pipelineNormalMap);

			unsigned int offset = 0;

			for (Mesh::Instance & object : m_rendering.m_aObjects)
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

				object.mesh->bind();

				for (SubMesh * m : object.getDrawCommands())
				{
					const GPU::Texture<GL_TEXTURE_2D> * pNormalMap = m->getNormalMap();

					if (nullptr != pNormalMap)
					{
						SetTexture(m_pipelineNormalMap.m_uShaderObject, "normalMap", 0, *pNormalMap, m_rendering.GetPipeline("geometry_normalmap")->m_mapSamplers["normalMap"]);

						SetUniform(m_pipelineNormalMap.m_uShaderObject, "shininess", m->m_material.shininess);

						m->draw(commandBuffer);
					}
				}

				object.mesh->unbind();

				++offset;
			}
		}
	}

	commandBuffer.EndRenderPass();

	return(true);
}
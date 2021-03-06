#include "RenderSceneToGBuffer.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
RenderSceneToGBuffer::RenderSceneToGBuffer() : GraphicsAlgorithm()
{
	m_loadOp = ATTACHMENT_LOAD_OP_CLEAR;
	m_fClearColorR = 0.0f;
	m_fClearColorG = 0.0f;
	m_fClearColorB = 0.0f;
	m_fClearColorA = 0.0f;

	m_depthLoadOp = ATTACHMENT_LOAD_OP_CLEAR;
	m_fClearDepthValue = 1.0f;
	m_iClearStencilValue = 0;
}

/**
 * @brief Destructor
 */
RenderSceneToGBuffer::~RenderSceneToGBuffer(void)
{
	// ...
}

/**
 * @brief RenderSceneToGBuffer::Create
 * @return
 */
RenderGraph::Operation * RenderSceneToGBuffer::Create()
{
	return new RenderSceneToGBuffer();
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
		RHI::PipelineInputAssemblyStateCreateInfo input;
		RHI::PipelineRasterizationStateCreateInfo rasterization;

		RHI::PipelineDepthStencilStateCreateInfo depthStencil;
		depthStencil.enableDepth = true;
		depthStencil.depthState.enableWrite = true;
		depthStencil.depthState.compareOp = RHI::COMPARE_OP_LESS;

		RHI::PipelineBlendStateCreateInfo blend;

		RHI::PipelineShaderStageCreateInfo vertexShaderSimple;
		vertexShaderSimple.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShaderSimple.module = m_rendering.m_mapShaderModules["geometry_pass.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShaderSimple;
		fragmentShaderSimple.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShaderSimple.module = m_rendering.m_mapShaderModules["geometry_pass.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStagesSimple;
		aStagesSimple.push_back(vertexShaderSimple);
		aStagesSimple.push_back(fragmentShaderSimple);

		RHI::PipelineShaderStageCreateInfo vertexShaderNM;
		vertexShaderNM.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShaderNM.module = m_rendering.m_mapShaderModules["geometry_normalmap_pass.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShaderNM;
		fragmentShaderNM.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShaderNM.module = m_rendering.m_mapShaderModules["geometry_normalmap_pass.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStagesNM;
		aStagesNM.push_back(vertexShaderNM);
		aStagesNM.push_back(fragmentShaderNM);

		m_pipelineSimple = RHI::Pipeline(input, rasterization, depthStencil, blend, aStagesSimple);
		m_pipelineNormalMap = RHI::Pipeline(input, rasterization, depthStencil, blend, aStagesNM);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo sampler;
		sampler.minFilter = RHI::FILTER_LINEAR;
		sampler.magFilter = RHI::FILTER_LINEAR;
		sampler.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_sampler = RHI::Sampler(sampler);
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
 * @brief RenderSceneToGBuffer::release
 * @return
 */
void RenderSceneToGBuffer::release(void)
{
	// TODO
}

/**
 * @brief RenderSceneToGBuffer::render
 * @param commandBuffer
 * @return
 */
bool RenderSceneToGBuffer::render(RenderGraph::Parameters & parameters, RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(RenderSceneToGBuffer);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), vec4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);

	{

		// OPTIMIZE THIS !!!!!
		{
			rmt_ScopedOpenGLSample(Simple);

			commandBuffer.Bind(m_pipelineSimple);

			unsigned int offset = 0;

			for (const Object & object : m_rendering.GetScene().getObjects())
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

				for (const Object::Mesh & mesh : object.Meshes)
				{
					if (0 == mesh.NormalMapID)
					{
						SetUniform(m_pipelineSimple.m_uShaderObject, "shininess", mesh.shininess);

						Mesh * pRenderingMesh = m_rendering.GetMesh(mesh.MeshID);

						pRenderingMesh->bind();

						for (SubMesh * m : pRenderingMesh->m_aSubMeshes)
						{
							m->draw(commandBuffer);
						}

						pRenderingMesh->unbind();
					}
				}

				++offset;
			}
		}

		{
			rmt_ScopedOpenGLSample(NormalMap);

			commandBuffer.Bind(m_pipelineNormalMap);

			unsigned int offset = 0;

			for (const Object & object : m_rendering.GetScene().getObjects())
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

				for (const Object::Mesh & mesh : object.Meshes)
				{
					if (0 != mesh.NormalMapID)
					{
						SetUniform(m_pipelineNormalMap.m_uShaderObject, "shininess", mesh.shininess);

						GLuint NormalMapId = m_rendering.GetTexture(mesh.NormalMapID);
						SetTexture<GL_TEXTURE_2D>(m_pipelineNormalMap.m_uShaderObject, "normalMap", 0, NormalMapId, m_sampler);

						Mesh * pRenderingMesh = m_rendering.GetMesh(mesh.MeshID);

						pRenderingMesh->bind();

						for (SubMesh * m : pRenderingMesh->m_aSubMeshes)
						{
							m->draw(commandBuffer);
						}

						pRenderingMesh->unbind();
					}
				}

				++offset;
			}
		}
	}

	commandBuffer.EndRenderPass();

	{
		GLint texture = 0;
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);

		RenderGraph::Value v;
		v.asUInt = texture;
		parameters.push(v);
	}

	{
		GLint texture = 0;
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);

		RenderGraph::Value v;
		v.asUInt = texture;
		parameters.push(v);
	}

	return(true);
}

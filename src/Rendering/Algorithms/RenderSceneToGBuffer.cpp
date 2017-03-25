#include "RenderSceneToGBuffer.h"

#include "Remotery.h"

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
 * @brief RenderSceneToGBuffer::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * RenderSceneToGBuffer::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new RenderSceneToGBuffer(rendering, framebuffer));
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
bool RenderSceneToGBuffer::release(void)
{
	return(false); // TODO
}

/**
 * @brief RenderSceneToGBuffer::render
 * @param commandBuffer
 * @return
 */
bool RenderSceneToGBuffer::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(RenderSceneToGBuffer);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), vec4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);

	{

		// OPTIMIZE THIS !!!!!
		{
			rmt_ScopedOpenGLSample(Simple);

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
			rmt_ScopedOpenGLSample(NormalMap);

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
						SetTexture(m_pipelineNormalMap.m_uShaderObject, "normalMap", 0, *pNormalMap, m_sampler);

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

/**
 * @brief RenderSceneToGBuffer::setParameter
 * @param name
 * @param value
 */
void RenderSceneToGBuffer::setParameter(const char * name, const char * value)
{
	assert(false);
}

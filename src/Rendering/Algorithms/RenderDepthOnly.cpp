#include "RenderDepthOnly.h"

#include "Remotery.h"

/**
 * @brief Constructor
 * @param rendering
 */
RenderDepthOnly::RenderDepthOnly(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
RenderDepthOnly::~RenderDepthOnly(void)
{
	// ...
}

/**
 * @brief RenderDepthOnly::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * RenderDepthOnly::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new RenderDepthOnly(rendering, framebuffer));
}

/**
 * @brief RenderDepthOnlyAlgorithm::init
 * @return
 */
bool RenderDepthOnly::init(void)
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

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = m_rendering.m_mapShaderModules["depth_only.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["depth_only.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		// no samplers
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief RenderDepthOnly::release
 * @return
 */
bool RenderDepthOnly::release(void)
{
	return(false); // TODO
}

/**
 * @brief RenderDepthOnly::render
 * @param commandBuffer
 * @return
 */
bool RenderDepthOnly::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(RenderDepthOnly);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), vec4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);

	{
		commandBuffer.Bind(m_pipeline);

		unsigned int offset = 0;

		for (Mesh::Instance & object : m_rendering.m_aObjects)
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

			object.mesh->bind();

			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++offset;
		}
	}

	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief RenderDepthOnly::setParameter
 * @param name
 * @param value
 */
void RenderDepthOnly::setParameter(const char * name, const char * value)
{
	assert(false);
}

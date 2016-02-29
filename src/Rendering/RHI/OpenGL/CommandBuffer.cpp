#include "../RHI.h"

/**
 * @brief Default constructor
 */
RHI::CommandBuffer::CommandBuffer(void)
: m_pCurrentPass(nullptr)
, m_pCurrentPipeline(nullptr)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Destructor
 */
RHI::CommandBuffer::~CommandBuffer(void)
{
	assert(!m_bActive);
}

/**
 * @brief RHI::CommandBuffer::Begin
 * @return
 */
bool RHI::CommandBuffer::Begin(void)
{
	m_bActive = true;

	return(true);
}

/**
 * @brief RHI::CommandBuffer::End
 * @return
 */
bool RHI::CommandBuffer::End(void)
{
	m_bActive = false;

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param color
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param depth
 * @param stencil
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, float depth, unsigned int stencil)
{
	assert(m_bActive);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glDepthMask(GL_TRUE);

	glClearDepthf(depth);
	glClearStencil(stencil);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::BeginRenderPass
 * @param pass
 * @param fb
 * @param offset
 * @param extent
 * @param color
 * @param depth
 * @param stencil
 * @return
 */
bool RHI::CommandBuffer::BeginRenderPass(RenderPass & pass, Framebuffer & fb, const ivec2 & offset, const ivec2 & extent, const vec4 & color, float depth, unsigned int stencil)
{
	assert(m_bActive);
	assert(nullptr == m_pCurrentPass);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.m_uFramebufferObject);

	m_pCurrentPass = &pass;
	m_pCurrentPass->Begin(0 == fb.m_uFramebufferObject);

	glViewport(offset.x, offset.y, extent.x, extent.y);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glClearColor(color.x, color.y, color.z, color.w);
	glClearDepthf(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return(true);
}

/**
 * @brief RHI::CommandBuffer::NextSubpass
 */
void RHI::CommandBuffer::NextSubpass(void)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);

	m_pCurrentPass->Next();
}

/**
 * @brief RHI::CommandBuffer::EndRenderPass
 */
void RHI::CommandBuffer::EndRenderPass(void)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_pCurrentPass->End();
	m_pCurrentPass = nullptr;
}

/**
 * @brief RHI::CommandBuffer::Bind
 * @param pipeline
 * @return
 */
bool RHI::CommandBuffer::Bind(Pipeline & pipeline)
{
	assert(m_bActive);

	m_pCurrentPipeline = &pipeline;

	//
	// Input Assembly State
	{
		const Pipeline::InputAssemblyState & state = pipeline.m_inputAssemblyState;

		if (state.enableRestartPrimitive)
		{
			glEnable(GL_PRIMITIVE_RESTART);
		}
		else
		{
			glDisable(GL_PRIMITIVE_RESTART);
		}
	}

	//
	// Depth State
	{
		const Pipeline::DepthStencilState & state = pipeline.m_depthStencilState;

		if (state.enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(state.depthCompareOp);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		glDepthMask(state.enableDepthWrite);
	}

	//
	// Blend State
	{
		const Pipeline::BlendState & state = pipeline.m_blendState;

		if (state.enable)
		{
			glEnable(GL_BLEND);
			glBlendEquationSeparate(state.colorEquation, state.alphaEquation);
			glBlendFuncSeparate(state.srcColorFactor, state.dstColorFactor, state.srcAlphaFactor, state.dstAlphaFactor);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		glColorMask(state.writeMask & COLOR_MASK_R, state.writeMask & COLOR_MASK_G, state.writeMask & COLOR_MASK_B, state.writeMask & COLOR_MASK_A);
	}

	// TODO : apply pipeline states

	return(true);
}

/**
 * @brief RHI::CommandBuffer::Draw
 * @param vertexCount
 * @param firstVertex
 * @return
 */
bool RHI::CommandBuffer::Draw(unsigned int vertexCount, unsigned int firstVertex)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode = GLenum(m_pCurrentPipeline->m_inputAssemblyState.topology);
	glDrawArrays(mode, firstVertex, vertexCount);
	return(true);
}

/**
 * @brief RHI::CommandBuffer::Draw
 * @param vertexCount
 * @param firstVertex
 * @param instanceCount
 * @return
 */
bool RHI::CommandBuffer::Draw(unsigned int vertexCount, unsigned int firstVertex, unsigned int instanceCount)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode = GLenum(m_pCurrentPipeline->m_inputAssemblyState.topology);
	glDrawArraysInstanced(mode, firstVertex, vertexCount, instanceCount);
	return(true);
}

/**
 * @brief RHI::CommandBuffer::Draw
 * @param vertexCount
 * @param firstVertex
 * @param instanceCount
 * @param firstInstance
 * @return
 */
bool RHI::CommandBuffer::Draw(unsigned int vertexCount, unsigned int firstVertex, unsigned int instanceCount, unsigned int firstInstance)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode(m_pCurrentPipeline->m_inputAssemblyState.topology);
	glDrawArraysInstancedBaseInstance(mode, firstVertex, vertexCount, instanceCount, firstInstance);
	return(true);
}

/**
 * @brief RHI::CommandBuffer::DrawIndexed
 * @param indexCount
 * @param firstIndex
 * @param vertexOffset
 * @return
 */
bool RHI::CommandBuffer::DrawIndexed(unsigned int indexCount, unsigned int firstIndex, unsigned int vertexOffset)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode(m_pCurrentPipeline->m_inputAssemblyState.topology);
	GLenum type = GL_UNSIGNED_INT; // FIXME : this should not be hardcoded !
	glDrawElementsBaseVertex(mode, indexCount, type, BUFFER_OFFSET(vertexOffset), firstIndex);
	return(true);
}

/**
 * @brief RHI::CommandBuffer::DrawIndexed
 * @param indexCount
 * @param firstIndex
 * @param vertexOffset
 * @param instanceCount
 * @return
 */
bool RHI::CommandBuffer::DrawIndexed(unsigned int indexCount, unsigned int firstIndex, unsigned int vertexOffset, unsigned int instanceCount)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode(m_pCurrentPipeline->m_inputAssemblyState.topology);
	GLenum type = GL_UNSIGNED_INT; // FIXME : this should not be hardcoded !
	glDrawElementsInstancedBaseVertex(mode, indexCount, type, BUFFER_OFFSET(vertexOffset), instanceCount, firstIndex);
	return(true);
}

/**
 * @brief RHI::CommandBuffer::DrawIndexed
 * @param indexCount
 * @param firstIndex
 * @param vertexOffset
 * @param instanceCount
 * @param firstInstance
 * @return
 */
bool RHI::CommandBuffer::DrawIndexed(unsigned int indexCount, unsigned int firstIndex, unsigned int vertexOffset, unsigned int instanceCount, unsigned int firstInstance)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);
	assert(nullptr != m_pCurrentPipeline);

	GLenum mode(m_pCurrentPipeline->m_inputAssemblyState.topology);
	GLenum type = GL_UNSIGNED_INT; // FIXME : this should not be hardcoded !
	glDrawElementsInstancedBaseVertexBaseInstance(mode, indexCount, type, BUFFER_OFFSET(vertexOffset), instanceCount, firstIndex, firstInstance);
	return(true);
}

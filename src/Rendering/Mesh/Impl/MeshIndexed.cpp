#include "MeshIndexed.h"

/**
 * @brief MeshIndexed::MeshIndexed
 * @param count
 * @param mode
 * @param offset
 * @param type
 * @param baseVertex
 */
MeshIndexed::MeshIndexed(GLsizei count, GLenum mode, unsigned int offset, GLenum type, unsigned int baseVertex)
: SubMesh(count, mode)
, m_eType(type)
, m_iOffset(offset)
, m_iBaseVertex(baseVertex)
{
	assert(GL_TRIANGLES == mode);
	assert(GL_UNSIGNED_INT == type);
}

/**
 * @brief Mesh::~Mesh
 */
MeshIndexed::~MeshIndexed(void)
{
	// ...
}

/**
 * @brief Mesh::draw
 */
void MeshIndexed::draw(RHI::CommandBuffer & commandBuffer) const
{
	commandBuffer.m_pCurrentPipeline->m_inputAssemblyState.topology = RHI::PrimitiveType(m_eMode); // TODO : remove this (ugly !!!)
	commandBuffer.DrawIndexed(m_iCount, m_iBaseVertex, m_iOffset);
}

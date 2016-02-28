#include "MeshSimple.h"

/**
 * @brief MeshSimple::MeshSimple
 * @param count
 * @param mode
 */
MeshSimple::MeshSimple(GLsizei count, GLenum mode)
: SubMesh(count, mode)
{
	// ...
}

/**
 * @brief MeshSimple::~MeshSimple
 */
MeshSimple::~MeshSimple(void)
{
	// ...
}

/**
 * @brief MeshSimple::draw
 */
void MeshSimple::draw(RHI::CommandBuffer & commandBuffer) const
{
	commandBuffer.m_pCurrentPipeline->m_inputAssemblyState.topology = RHI::PrimitiveType(m_eMode); // TODO : remove this (ugly !!!)
	commandBuffer.Draw(m_iCount, 0);
}

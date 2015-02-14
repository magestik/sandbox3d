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
	// ...
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
void MeshIndexed::draw(void) const
{
	glDrawElementsBaseVertex(m_eMode, m_iCount, m_eType, BUFFER_OFFSET(m_iOffset), m_iBaseVertex);
}

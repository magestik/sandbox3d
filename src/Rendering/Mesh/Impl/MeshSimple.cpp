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
void MeshSimple::draw(void) const
{
	glDrawArrays(m_eMode, 0, m_iCount);
}

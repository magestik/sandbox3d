#include "Mesh.h"

#include "Impl/MeshSimple.h"
#include "Impl/MeshIndexed.h"

/**
 * @brief SubMesh::SubMesh
 * @param iCount
 * @param eMode
 */
SubMesh::SubMesh(GLsizei iCount, GLenum eMode)
: m_iCount(iCount)
, m_eMode(eMode)
, m_NormalMapId(0)
{
	m_material.m_diffuse = 0;
	m_material.m_specular = 0;
	m_material.shininess = 0.0f;
}

/**
 * @brief SubMesh::~SubMesh
 */
SubMesh::~SubMesh()
{
	// ...
}

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
, m_pNormalMap(nullptr)
{
	m_material.m_diffuse = nullptr;
	m_material.m_specular = nullptr;
	m_material.shininess = 0.0f;
}

/**
 * @brief SubMesh::~SubMesh
 */
SubMesh::~SubMesh()
{
	// ...
}

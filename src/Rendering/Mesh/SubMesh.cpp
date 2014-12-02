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
	glGenVertexArrays(1, &m_uObject);
}

/**
 * @brief SubMesh::~SubMesh
 */
SubMesh::~SubMesh()
{
	glDeleteVertexArrays(1, &m_uObject);
	m_uObject = 0;
}

/**
 * @brief Mesh::Create
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @return
 */
SubMesh * SubMesh::Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs)
{
	SubMesh * pMesh = new MeshSimple(pVertexBuffer, count, mode, specs);
	return(pMesh);
}

/**
 * @brief Mesh::Create
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @param pIndexBuffer
 * @param type
 * @return
 */
SubMesh * SubMesh::Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int firstIndex, GLenum type)
{
	SubMesh * pMesh = new MeshIndexed(pVertexBuffer, count, mode, specs, pIndexBuffer, firstIndex, type);
	return(pMesh);
}

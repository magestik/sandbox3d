#include "Mesh.h"

#include "Impl/MeshSimple.h"
#include "Impl/MeshIndexed.h"

/**
 * @brief Mesh::Mesh
 * @param uObject
 * @param iCount
 * @param eMode
 */
Mesh::Mesh(GLsizei iCount, GLenum eMode)
: m_iCount(iCount)
, m_eMode(eMode)
{
	glGenVertexArrays(1, &m_uObject);
}

/**
 * @brief Mesh::~Mesh
 */
Mesh::~Mesh()
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
Mesh * Mesh::Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs)
{
	Mesh * pMesh = new MeshSimple(pVertexBuffer, count, mode, specs);
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
Mesh * Mesh::Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, GLenum type)
{
	Mesh * pMesh = new MeshIndexed(pVertexBuffer, count, mode, specs, pIndexBuffer, type);
	return(pMesh);
}

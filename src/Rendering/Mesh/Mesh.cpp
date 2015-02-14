#include "Mesh.h"

#include "Impl/MeshSimple.h"
#include "Impl/MeshIndexed.h"

/**
 * @brief Mesh::Mesh
 */
Mesh::Mesh(void)
: m_aSubMeshes()
{
	// ...
}

/**
 * @brief Mesh::Mesh
 * @param submeshes
 */
Mesh::Mesh(const std::vector<SubMesh*> & submeshes)
: m_aSubMeshes(submeshes)
{
	// ...
}

/**
 * @brief Mesh::~Mesh
 */
Mesh::~Mesh()
{
	// ...
}

/**
 * @brief Mesh::Create
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @return
 */
void Mesh::AddSubMesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs)
{
	SubMesh * pMesh = new MeshSimple(pVertexBuffer, count, mode, specs);
	m_aSubMeshes.push_back(pMesh);
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
void Mesh::AddSubMesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int firstIndex, GLenum type, unsigned int base_vertex)
{
	SubMesh * pMesh = new MeshIndexed(pVertexBuffer, count, mode, specs, pIndexBuffer, firstIndex, type, base_vertex);
	m_aSubMeshes.push_back(pMesh);
}

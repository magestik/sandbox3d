#include "Mesh.h"

#include "Impl/MeshSimple.h"
#include "Impl/MeshIndexed.h"

/**
 * @brief Mesh::Mesh
 * @param pVertexBuffer
 * @param specs
 */
Mesh::Mesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, const std::vector<Mesh::VertexSpec> & specs)
: m_aSubMeshes()
, m_uObject(0)
, m_isBound(false)
{
	glGenVertexArrays(1, &m_uObject);

	glBindVertexArray(m_uObject);

	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->GetObject());

	for (const Mesh::VertexSpec & spec : specs)
	{
		glEnableVertexAttribArray(spec.index);
		glVertexAttribPointer(spec.index, spec.size, spec.type, spec.normalized, spec.stride, spec.pointer);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0); // The GL_ARRAY_BUFFER​ binding is NOT part of the VAO's state!

	glBindVertexArray(0);
}

/**
 * @brief Mesh::Mesh
 * @param pVertexBuffer
 * @param specs
 * @param pIndexBuffer
 * @param type
 */
Mesh::Mesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, const std::vector<Mesh::VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer)
: m_aSubMeshes()
, m_uObject(0)
, m_isBound(false)
{
	glGenVertexArrays(1, &m_uObject);

	glBindVertexArray(m_uObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetObject());

	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->GetObject());

	for (const Mesh::VertexSpec & spec : specs)
	{
		glEnableVertexAttribArray(spec.index);
		glVertexAttribPointer(spec.index, spec.size, spec.type, spec.normalized, spec.stride, spec.pointer);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0); // The GL_ARRAY_BUFFER​ binding is NOT part of the VAO's state!

	glBindVertexArray(0);
}

/**
 * @brief Mesh::~Mesh
 */
Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &m_uObject);
}

/**
 * @brief Mesh::Create
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @return
 */
SubMesh * Mesh::AddSubMesh(GLsizei count, GLenum mode)
{
	SubMesh * pMesh = new MeshSimple(count, mode);
	m_aSubMeshes.push_back(pMesh);
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
SubMesh * Mesh::AddSubMesh(GLsizei count, GLenum mode, unsigned int firstIndex, GLenum type, unsigned int base_vertex)
{
	SubMesh * pMesh = new MeshIndexed(count, mode, firstIndex, type, base_vertex);
	m_aSubMeshes.push_back(pMesh);
	return(pMesh);
}

/**
 * @brief Mesh::bind
 */
void Mesh::bind(void)
{
	assert(!m_isBound);
	glBindVertexArray(m_uObject);
	m_isBound = true;
}

/**
 * @brief Mesh::unbind
 */
void Mesh::unbind(void)
{
	assert(m_isBound);
	glBindVertexArray(0);
	m_isBound = false;
}

/**
 * @brief Mesh::draw
 */
void Mesh::draw(void)
{
	bind();

	for (SubMesh * m : m_aSubMeshes)
	{
		m->draw();
	}

	unbind();
}

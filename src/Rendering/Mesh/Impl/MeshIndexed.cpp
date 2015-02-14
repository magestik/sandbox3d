#include "MeshIndexed.h"

/**
 * @brief MeshIndexed::MeshIndexed
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @param pIndexBuffer
 * @param type
 */
MeshIndexed::MeshIndexed(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int offset, GLenum type, unsigned int baseVertex)
: SubMesh(count, mode)
, m_eType(type)
, m_iOffset(offset)
, m_iBaseVertex(baseVertex)
{
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
MeshIndexed::~MeshIndexed(void)
{

}

/**
 * @brief Mesh::draw
 */
void MeshIndexed::draw(void) const
{
	glBindVertexArray(m_uObject);
	glDrawElementsBaseVertex(m_eMode, m_iCount, m_eType, BUFFER_OFFSET(m_iOffset), m_iBaseVertex);
	glBindVertexArray(0);
}

#include "MeshSimple.h"

/**
 * @brief MeshSimple::MeshSimple
 * @param pVertexBuffer
 * @param count
 * @param mode
 * @param specs
 * @param pIndexBuffer
 * @param type
 */
MeshSimple::MeshSimple(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs)
: SubMesh(count, mode)
{
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
 * @brief MeshSimple::~MeshSimple
 */
MeshSimple::~MeshSimple(void)
{

}

/**
 * @brief MeshSimple::draw
 */
void MeshSimple::draw(void) const
{
	glBindVertexArray(m_uObject);
	glDrawArrays(m_eMode, 0, m_iCount);
	glBindVertexArray(0);
}

#pragma once

#include "Mesh.h"

class SubMesh
{
public:

	struct VertexSpec
	{
		GLuint		index;
		GLint		size;
		GLenum		type;
		GLboolean	normalized;
		GLsizei		stride;
		GLvoid *	pointer;
	};

	virtual ~SubMesh(void);

	virtual void draw(void) const = 0;

	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs);
	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, GLenum type);

protected:

	explicit SubMesh(GLsizei iCount, GLenum eMode);

	GLuint m_uObject;

	GLsizei m_iCount;
	GLenum m_eMode;
};

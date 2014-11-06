#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>

class Mesh
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

	struct Instance
	{
		Instance(const Mesh * m) : transformation(1.0f), mesh(m) { /* ... */ }
		mat4x4 transformation;
		const Mesh * mesh;
	};

	virtual ~Mesh(void);

	virtual void draw(void) const = 0;

	static Mesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs);
	static Mesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, GLenum type);

	Instance Instantiate() const { return(Instance(this)); }

protected:

	explicit Mesh(GLsizei iCount, GLenum eMode);

	GLuint m_uObject;

	GLsizei m_iCount;
	GLenum m_eMode;
};

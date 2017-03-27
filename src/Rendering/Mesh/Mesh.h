#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>

#include "RHI/RHI.h"

class SubMesh;

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

	struct BoundingBox
	{
		BoundingBox() : min(1e10f, 1e10f, 1e10f), max(-1e10f, -1e10f, -1e10f) { /* ... */ }
		vec3 min;
		vec3 max;
	};

	explicit Mesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, const std::vector<VertexSpec> & specs);
	explicit Mesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer);
	virtual ~Mesh(void);

	SubMesh * AddSubMesh(GLsizei count, GLenum mode);
	SubMesh * AddSubMesh(GLsizei count, GLenum mode, unsigned int firstIndex, GLenum type, unsigned int base_vertex = 0);

	void bind();
	void unbind();

	void draw(RHI::CommandBuffer & commandBuffer);

//private:

	std::vector<SubMesh*> m_aSubMeshes;

private:

	GLuint m_uObject;

	bool m_isBound;

public:

	BoundingBox m_BoundingBox;
};

#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>

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

	struct Instance
	{
		Instance(const Mesh * m) : transformation(1.0f), mesh(m) { /* ... */ }
		const std::vector<SubMesh*> & getDrawCommands() const { return(mesh->m_aSubMeshes); }
		mat4x4 transformation;
		const Mesh * mesh;
	};

	explicit Mesh(void);
	explicit Mesh(const std::vector<SubMesh*> & submeshes);
	virtual ~Mesh(void);

	void AddSubMesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs);
	void AddSubMesh(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int firstIndex, GLenum type, unsigned int base_vertex = 0);


	Instance Instantiate() const { return(Instance(this)); }

//private:

	std::vector<SubMesh*> m_aSubMeshes;
};

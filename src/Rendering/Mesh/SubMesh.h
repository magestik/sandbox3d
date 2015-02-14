#pragma once

#include "Mesh.h"

class SubMesh
{
public:

	struct VertexSimple
	{
		vec3 position;
		vec2 uv;
		vec3 normal;
		vec3 tangent;
	};

	typedef struct {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int firstIndex;
		unsigned int baseVertex;
		unsigned int baseInstance;
	} DrawElementsIndirectCommand;

	struct Material
	{
		GPU::Texture<GL_TEXTURE_2D> * m_diffuse;
		GPU::Texture<GL_TEXTURE_2D> * m_specular;
		float shininess;
	};

	explicit SubMesh(GLsizei iCount, GLenum eMode);
	virtual ~SubMesh(void);

	virtual void draw(void) const = 0;

	const GPU::Texture<GL_TEXTURE_2D> * getNormalMap(void) { return(m_pNormalMap); }

	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs);
	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int firstIndex, GLenum type, unsigned int base_vertex = 0);

protected:

	void bindStuff(void);
	void unbindStuff(void);

	GLuint m_uObject;

	GLsizei m_iCount;
	GLenum m_eMode;

public:
	Material m_material;

	const GPU::Texture<GL_TEXTURE_2D> * m_pNormalMap;

	vec3 m_vMin;
	vec3 m_vMax;
};

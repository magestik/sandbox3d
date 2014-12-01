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
	};

	virtual ~SubMesh(void);

	virtual void draw(void) const = 0;

	const GPU::Texture<GL_TEXTURE_2D> * getNormalMap(void) { return(m_pNormalMap); }

	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs);
	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int firstIndex, GLenum type);

protected:

	explicit SubMesh(GLsizei iCount, GLenum eMode);

	void bindStuff(void);
	void unbindStuff(void);

	GLuint m_uObject;

	GLsizei m_iCount;
	GLenum m_eMode;

	Material m_material;
public:
	const GPU::Texture<GL_TEXTURE_2D> * m_pNormalMap;
};

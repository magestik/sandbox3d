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
	};

	struct VertexWithTangent
	{
		vec3 position;
		vec2 uv;
		vec3 normal;
		vec3 tangent;
	};

	struct Material
	{
		GPU::Texture<GL_TEXTURE_2D> m_diffuse;
		GPU::Texture<GL_TEXTURE_2D> m_specular;
		GPU::Texture<GL_TEXTURE_2D> m_normal;
	};

	virtual ~SubMesh(void);

	virtual void draw(void) const = 0;

	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs);
	static SubMesh * Create(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, unsigned int offset, GLenum type);

protected:

	explicit SubMesh(GLsizei iCount, GLenum eMode);

	void bindStuff(void);
	void unbindStuff(void);

	GLuint m_uObject;

	GLsizei m_iCount;
	GLenum m_eMode;

	Material m_material;
	bool m_bHasNormalMap;
};

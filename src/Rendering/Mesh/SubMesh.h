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

protected:

	GLsizei m_iCount;
	GLenum m_eMode;

public:

	Material m_material;

	const GPU::Texture<GL_TEXTURE_2D> * m_pNormalMap;
};

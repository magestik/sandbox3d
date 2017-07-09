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
		GLuint m_diffuse;
		GLuint m_specular;
		float shininess;
	};

	explicit SubMesh(GLsizei iCount, GLenum eMode);
	virtual ~SubMesh(void);

	virtual void draw(RHI::CommandBuffer & commandBuffer) const = 0;
	virtual void drawGL(void) const = 0;

	const GLuint getNormalMap(void) const { return(m_NormalMapId); }

protected:

	GLsizei m_iCount;
	GLenum m_eMode;

public:

	Material m_material;

	GLuint m_NormalMapId;
};

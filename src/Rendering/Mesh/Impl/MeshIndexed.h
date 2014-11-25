#pragma once

#include "../SubMesh.h"

class MeshIndexed : public SubMesh
{
public:

	explicit MeshIndexed(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<VertexSpec> & specs, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * pIndexBuffer, GLenum type);
	virtual ~MeshIndexed(void);

	virtual void draw(void) const;

private:

	GLenum m_eType;

};

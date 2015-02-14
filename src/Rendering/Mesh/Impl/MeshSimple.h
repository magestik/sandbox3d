#pragma once

#include "../SubMesh.h"

class MeshSimple : public SubMesh
{
public:

	explicit MeshSimple(GPU::Buffer<GL_ARRAY_BUFFER> * pVertexBuffer, GLsizei count, GLenum mode, const std::vector<Mesh::VertexSpec> & specs);
	virtual ~MeshSimple(void);

	virtual void draw(void) const;

private:

	// ...

};

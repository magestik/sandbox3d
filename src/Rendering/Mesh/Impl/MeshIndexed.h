#pragma once

#include "../SubMesh.h"

class MeshIndexed : public SubMesh
{
public:

	explicit MeshIndexed(GLsizei count, GLenum mode, unsigned int offset, GLenum type, unsigned int baseVertex);
	virtual ~MeshIndexed(void);

	virtual void draw(RHI::CommandBuffer & commandBuffer) const;
	virtual void drawGL(void) const;

private:

	GLenum m_eType;
	unsigned int m_iOffset;
	unsigned int m_iBaseVertex;
};

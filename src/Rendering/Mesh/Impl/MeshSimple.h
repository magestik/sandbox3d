#pragma once

#include "../SubMesh.h"

class MeshSimple : public SubMesh
{
public:

	explicit MeshSimple(GLsizei count, GLenum mode);
	virtual ~MeshSimple(void);

	virtual void draw(void) const;

private:

	// ...

};

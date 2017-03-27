#pragma once

#include "../Mesh/Mesh.h"

struct Object
{
	Object(Mesh * m) : transformation(1.0f), mesh(m) { /* ... */ }
	const std::vector<SubMesh*> & getDrawCommands() const { return(mesh->m_aSubMeshes); }
	mat4x4 transformation;
	Mesh * mesh;
};

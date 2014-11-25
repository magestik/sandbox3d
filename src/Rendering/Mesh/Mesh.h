#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>

class SubMesh;

class Mesh
{

public:

	struct Instance
	{
		Instance(const Mesh * m) : transformation(1.0f), mesh(m) { /* ... */ }
		mat4x4 transformation;
		const Mesh * mesh;
	};

	explicit Mesh(void);
	explicit Mesh(const std::vector<SubMesh*> & submeshes);
	virtual ~Mesh(void);

	void draw(void) const;

	Instance Instantiate() const { return(Instance(this)); }

private:

	std::vector<SubMesh*> m_aSubMeshes;
};

#include "Mesh.h"

#include "Impl/MeshSimple.h"
#include "Impl/MeshIndexed.h"

/**
 * @brief Mesh::Mesh
 */
Mesh::Mesh(void)
: m_aSubMeshes()
{
	// ...
}

/**
 * @brief Mesh::Mesh
 * @param submeshes
 */
Mesh::Mesh(const std::vector<SubMesh*> & submeshes)
: m_aSubMeshes(submeshes)
{
	// ...
}

/**
 * @brief Mesh::~Mesh
 */
Mesh::~Mesh()
{
	// ...
}

/**
 * @brief Mesh::draw
 */
void Mesh::draw() const
{
	for (SubMesh * m : m_aSubMeshes)
	{
		m->draw();
	}
}

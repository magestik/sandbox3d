#include "MeshLoader.h"
#include "TextureLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../Rendering/Rendering.h"

void loadAllMaterials(const aiScene * scene, const QDir & dir)
{
	aiTextureType aSupportedTextureTypes [] = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_HEIGHT };

	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial * material = scene->mMaterials[i];

		for (aiTextureType type : aSupportedTextureTypes)
		{
			aiString str;

			material->GetTexture(type, 0, &str);

			if (g_Textures.find(str.C_Str()) == g_Textures.end())
			{
				QString texture_filename(str.C_Str());
				GPU::Texture<GL_TEXTURE_2D> * texture = loadTexture(dir, texture_filename);

				if (texture != nullptr)
				{
					g_Textures.insert(std::pair<std::string, GPU::Texture<GL_TEXTURE_2D> *>(texture_filename.toStdString(), texture));
				}
			}
		}
	}
}

Mesh loadMesh(const QDir & dir, const QString & filename)
{
	std::string filepath = dir.filePath(filename).toStdString();

	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(filepath.data(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		return(Mesh());
	}

	loadAllMaterials(scene, dir);

	std::vector<SubMesh*> meshes;

	uint NumVertices = 0;
	uint NumIndices = 0;

	// Count the number of vertices and indices
	for (int i = 0 ; i < scene->mNumMeshes ; i++)
	{
		NumVertices += scene->mMeshes[i]->mNumVertices;
		NumIndices  += scene->mMeshes[i]->mNumFaces;
	}

	// Reserve GPU memory for the vertex attributes and indices
	GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();
	GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer = new GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER>();

	GPU::realloc(*vertexBuffer, NumVertices * sizeof(SubMesh::VertexSimple));
	GPU::realloc(*indexBuffer, NumIndices * 3 * sizeof(unsigned int));

	void * pVertexGPU = GPU::mmap(*vertexBuffer, GL_WRITE_ONLY);
	void * pIndexGPU = GPU::mmap(*indexBuffer, GL_WRITE_ONLY);

	unsigned int vertex_offset = 0;
	unsigned int index_offset = 0;

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh * mesh = scene->mMeshes[i];

		std::vector<SubMesh::VertexSimple> vertices;
		std::vector<unsigned int> triangles;

		vertices.reserve(mesh->mNumVertices);
		triangles.reserve(mesh->mNumFaces*3);

		aiString str;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, 0, &str);
		std::string texture_name(str.C_Str());

		bool hasNormalMap = (g_Textures.find(texture_name) != g_Textures.end()) && mesh->HasTangentsAndBitangents();

		// Populate the vertex attribute vectors
		for (int j = 0 ; j < mesh->mNumVertices ; ++j)
		{
			SubMesh::VertexSimple vertex;

			vertex.position.x = mesh->mVertices[j].x;
			vertex.position.y = mesh->mVertices[j].y;
			vertex.position.z = mesh->mVertices[j].z;

			if (hasNormalMap)
			{
				vec3 tangent = vec3(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				vec3 bitangent = vec3(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z);
				vec3 normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

				mat3x3 TBN = transpose(mat3x3(normal, bitangent, tangent));

				float r = 0.5f * sqrt(1 + TBN[0][0] + TBN[1][1] + TBN[2][2]);
				vec4 q = normalize(vec4(r, 0.25f * (TBN[2][1] - TBN[1][2]), 0.25f * (TBN[0][2] - TBN[2][0]), 0.25f * (TBN[1][0] - TBN[0][1])));

				if (q.w < 0.0f)
				{
					vertex.normal.x = -q.x;
					vertex.normal.y = -q.y;
					vertex.normal.z = -q.z;
				}
				else
				{
					vertex.normal.x = q.x;
					vertex.normal.y = q.y;
					vertex.normal.z = q.z;
				}
			}
			else
			{
				vertex.normal.x = mesh->mNormals[j].x;
				vertex.normal.y = mesh->mNormals[j].y;
				vertex.normal.z = mesh->mNormals[j].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.uv.x = mesh->mTextureCoords[0][j].x;
				vertex.uv.y = mesh->mTextureCoords[0][j].y;
			}
			else
			{
				vertex.uv.x = 0.5f;
				vertex.uv.y = 0.5f;
			}

			vertices.push_back(vertex);
		}

		// Populate the index buffer
		for (int j = 0 ; j < mesh->mNumFaces ; ++j)
		{
			const aiFace & Face = mesh->mFaces[j];
			triangles.push_back(Face.mIndices[0]);
			triangles.push_back(Face.mIndices[1]);
			triangles.push_back(Face.mIndices[2]);
		}

		memcpy(pVertexGPU, (void *)vertices.data(), vertices.size() * sizeof(SubMesh::VertexSimple));
		memcpy(pIndexGPU, (void *)triangles.data(), triangles.size() * sizeof(unsigned int));

		pVertexGPU = (void*)(((char*)pVertexGPU) + vertices.size() * sizeof(SubMesh::VertexSimple));
		pIndexGPU = (void*)(((char*)pIndexGPU) + triangles.size() * sizeof(unsigned int));

		std::vector<SubMesh::VertexSpec> specs;

		SubMesh::VertexSpec SPEC_POS;
		SPEC_POS.index = 0;
		SPEC_POS.size = 3;
		SPEC_POS.type = GL_FLOAT;
		SPEC_POS.normalized = GL_FALSE;
		SPEC_POS.stride = sizeof(SubMesh::VertexSimple);
		SPEC_POS.pointer = BUFFER_OFFSET(vertex_offset);

		SubMesh::VertexSpec SPEC_UV;
		SPEC_UV.index = 2;
		SPEC_UV.size = 2;
		SPEC_UV.type = GL_FLOAT;
		SPEC_UV.normalized = GL_FALSE;
		SPEC_UV.stride = sizeof(SubMesh::VertexSimple);
		SPEC_UV.pointer = BUFFER_OFFSET(vertex_offset+sizeof(float)*3);

		SubMesh::VertexSpec SPEC_NORMAL;
		SPEC_NORMAL.index = 1;
		SPEC_NORMAL.size = 3;
		SPEC_NORMAL.type = GL_FLOAT;
		SPEC_NORMAL.normalized = GL_FALSE;
		SPEC_NORMAL.stride = sizeof(SubMesh::VertexSimple);
		SPEC_NORMAL.pointer = BUFFER_OFFSET(vertex_offset+sizeof(float)*5);

		specs.push_back(SPEC_POS);
		specs.push_back(SPEC_UV);
		specs.push_back(SPEC_NORMAL);

		SubMesh * submesh = SubMesh::Create(vertexBuffer, triangles.size(), GL_TRIANGLES, specs, indexBuffer, index_offset, GL_UNSIGNED_INT);
		meshes.push_back(submesh);

		if (hasNormalMap)
		{
			submesh->m_pNormalMap = g_Textures[texture_name];
		}

		vertex_offset += vertices.size() * sizeof(SubMesh::VertexSimple);
		index_offset += triangles.size() * sizeof(unsigned int);
	}

	GPU::munmap(*vertexBuffer);
	GPU::munmap(*indexBuffer);

	return(Mesh(meshes));
}

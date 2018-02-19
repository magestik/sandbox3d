#include "Rendering.h"

#include "Remotery.h"

#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.inl"

#include "RenderXML.h"

#include "Algorithms/RenderSceneToGBuffer.h"
#include "Algorithms/RenderSceneToShadowMap.h"
#include "Algorithms/RenderLightsToAccumBuffer.h"
#include "Algorithms/Compose.h"
#include "Algorithms/Fog.h"
#include "Algorithms/BrightFilter.h"
#include "Algorithms/ToneMapping.h"
#include "Algorithms/FXAA.h"
#include "Algorithms/BlurH.h"
#include "Algorithms/BlurV.h"
#include "Algorithms/Bloom.h"

#define _min(x, y) ((x < y) ? x : y)
#define _max(x, y) ((x > y) ? x : y)

#define MAX_Z 1000.0f
#define MIN_Z 0.01f

#define ENABLE_PICKBUFFER 1

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

// sRGB
const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

// Adobe RGB
const mat3x3 RGB_to_XYZ(0.5767309, 0.1855540, 0.1881852, 0.2973769, 0.6273491, 0.0752741, 0.0270343, 0.0706872, 0.9911085);
const mat3x3 XYZ_to_RGB(2.0413690, -0.5649464, -0.3446944, -0.9692660, 1.8760108, 0.0415560, 0.0134474, -0.1183897, 1.0154096);

static inline unsigned int toPOT(unsigned int v)
{
	unsigned r = 1;

	while (r < v)
	{
		r <<= 1;
	}

	return(r);
}

/**
 * @brief Rendering::onInitialize
 */
Rendering::Rendering(Scene & scene)
: m_uWidth(1280)
, m_uHeight(720)
, m_fMinZ(1.0f)
, m_fMaxZ(1000.0f)
, m_pQuadMesh(nullptr)
, m_pCameraBuffer(nullptr)
, m_pObjectsBuffer(nullptr)
, m_mapTargets()
, m_bReady(false)
, m_bInitialized(false)
, m_scene(scene)
{
	GraphicsAlgorithm::RegisterEverything();
	m_scene.registerListener(this);
	m_scene.getResourceManager().registerListener(this);
}

/**
 * @brief Rendering::onReady
 */
void Rendering::onReady(const char * szBaseDir)
{
	assert(!m_bReady);

	initShaders(szBaseDir);

	//
	// Generate Meshes
	generateMeshes();

	//
	// Initialize Uniform blocks
	m_pCameraBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
	GPU::realloc(*m_pCameraBuffer, sizeof(CameraBlock), GL_STREAM_DRAW);

	m_pObjectsBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
	GPU::realloc(*m_pObjectsBuffer, sizeof(ObjectBlock)*m_scene.getObjectCount(), GL_STREAM_DRAW);

	glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_CAMERA, m_pCameraBuffer->GetObject(), 0, sizeof(CameraBlock));

	rmt_BindOpenGL();

	m_bReady = true;
}

/**
 * @brief Rendering::initQueue
 */
void Rendering::initQueue(const char * szFilename)
{
	if (!m_bReady)
	{
		return;
	}

	if (m_bInitialized)
	{
		releaseQueue();
	}

	RenderXML renderXML(szFilename);

	renderXML.initializeTargets(*this);

	onResize(m_uWidth, m_uHeight);

	renderXML.initializeFramebuffers(*this);

	renderXML.initializeQueue(*this);

	for (GraphicsAlgorithm * qElmt : m_renderQueue)
	{
		qElmt->init();
	}

	m_bInitialized = true;
}

/**
 * @brief Rendering::initQueue
 */
void Rendering::releaseQueue(void)
{
	for (GraphicsAlgorithm * qElmt : m_renderQueue)
	{
		qElmt->release();
		delete qElmt;
	}

	m_renderQueue.clear();

	// TODO : delete framebuffers

	m_mapFramebuffer.clear();

	// TODO : delete targets

	m_mapTargets.clear();
}

/**
 * @brief Rendering::initShaders
 */
void Rendering::initShaders(const char * szShadersDir)
{
	DIR * dir = opendir(szShadersDir);

	if (dir != nullptr)
	{
		struct dirent *ep;
		while ((ep = readdir(dir)))
		{
			std::string filename(ep->d_name);

			RHI::ShaderStage stage = RHI::ShaderStage(0);

			if (ends_with(filename, "vert"))
			{
				stage = RHI::SHADER_STAGE_VERTEX;
			}
			else if (ends_with(filename, "frag"))
			{
				stage = RHI::SHADER_STAGE_FRAGMENT;
			}
			else if (ends_with(filename, "geom"))
			{
				stage = RHI::SHADER_STAGE_GEOMETRY;
			}

			if (0 != stage)
			{
			        std::string strFilePath = std::string(szShadersDir) + "/" + ep->d_name;
				int fd = open(strFilePath.c_str(), O_RDONLY);

				struct stat buf;
				fstat(fd, &buf);

				const unsigned int length = buf.st_size;
				char * data = new char [length];

				read(fd, (void*)data, length);

				RHI::ShaderModuleCreateInfo createInfo;
				createInfo.stage = stage;
				createInfo.pCode = data;
				createInfo.codeSize = length;

				RHI::ShaderModule module(createInfo);
				m_mapShaderModules.insert(std::pair<std::string, RHI::ShaderModule>(filename, module));

				delete [] data;

				close(fd);
			}
		}

		closedir(dir);
	}
}

/**
 * @brief Generate simple mesh used for rendering (ie fullscreen quad, light volumes ...)
 */
void Rendering::generateMeshes()
{
	//
	// Full Screen Quad
	//
	{
		float points [] =
		{
			-1.0f, -1.0f, /* | */ 0.0f, 0.0f,
			 1.0f, -1.0f, /* | */ 1.0f, 0.0f,
			-1.0f,  1.0f, /* | */ 0.0f, 1.0f,
			 1.0f,  1.0f, /* | */ 1.0f, 1.0f,
		};

		GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();

		GPU::realloc(*vertexBuffer, sizeof(points), GL_STATIC_DRAW, points);

		std::vector<Mesh::VertexSpec> specs;

		Mesh::VertexSpec SPEC_POS;
		SPEC_POS.index = 0;
		SPEC_POS.size = 2;
		SPEC_POS.type = GL_FLOAT;
		SPEC_POS.normalized = GL_FALSE;
		SPEC_POS.stride = 4 * sizeof(float);
		SPEC_POS.pointer = 0;

		Mesh::VertexSpec SPEC_UV;
		SPEC_UV.index = 2;
		SPEC_UV.size = 2;
		SPEC_UV.type = GL_FLOAT;
		SPEC_UV.normalized = GL_FALSE;
		SPEC_UV.stride = 4 * sizeof(float);
		SPEC_UV.pointer = (void*)(sizeof(float)*2);

		specs.push_back(SPEC_POS);
		specs.push_back(SPEC_UV);

		m_pQuadMesh = new Mesh(vertexBuffer, specs);
		m_pQuadMesh->AddSubMesh(4, GL_TRIANGLE_STRIP);
		//SubMesh::Create(vertexBuffer, 4, GL_TRIANGLE_STRIP, specs);
	}

	// TODO : Sphere / Cone / Cube
}

/**
 * @brief Rendering::updateCameraBuffer
 * @param matView
 */
void Rendering::updateCameraBuffer(const mat4x4 & matView)
{
	const BoundingBox & box = m_scene.getBoundingBox();
	const BoundingSphere & sphere = m_scene.getBoundingSphere();

	vec3 points [8] =
	{
		vec3(box.min.x, box.min.y, box.min.z),
		vec3(box.max.x, box.min.y, box.min.z),
		vec3(box.min.x, box.max.y, box.min.z),
		vec3(box.max.x, box.max.y, box.min.z),
		vec3(box.min.x, box.min.y, box.max.z),
		vec3(box.max.x, box.min.y, box.max.z),
		vec3(box.min.x, box.max.y, box.max.z),
		vec3(box.max.x, box.max.y, box.max.z),
	};

	float viewMin = INFINITY;
	float viewMax = -INFINITY;

	for (const vec3 & point : points)
	{
		vec3 ViewPoint = _transform(point, matView);

		float len = dot(vec3(0.0f, 0.0f, -1.0f), ViewPoint); // distance along the camera dir

		if (len < viewMin)
		{
			viewMin = len;
		}

		if (len > viewMax)
		{
			viewMax = len;
		}
	}

	m_fMinZ = (viewMin);
	m_fMaxZ = (viewMax);

	if (m_fMinZ < MIN_Z)
	{
		m_fMinZ = MIN_Z;
	}

	if (m_fMaxZ > MAX_Z)
	{
		m_fMaxZ = MAX_Z;
	}

	const float fov = 75.0f;

	// Compute the new Projection Matrix
	float ratio = m_uWidth/(float)m_uHeight;
	m_matProjection = _perspective(fov, ratio, m_fMinZ, m_fMaxZ);

	CameraBlock cam;
	// matrix
	cam.View = matView;
	cam.Projection = m_matProjection;
	cam.ViewProjection = m_matProjection * matView;
	// inverse matrix
	cam.InverseView = inverse(cam.View);
	cam.InverseProjection = inverse(cam.Projection);
	cam.InverseViewProjection = inverse(cam.ViewProjection);
	// View parameters
	cam.aspectRatio = ratio;
	cam.tanHalfFOV = tanf(radians(fov * 0.5f));
	GPU::memcpy(*m_pCameraBuffer, &cam, sizeof(CameraBlock));
}

/**
 * @brief Rendering::updateObjectsBuffer
 */
void Rendering::updateObjectsBuffer(void)
{
	GPU::realloc(*m_pObjectsBuffer, sizeof(ObjectBlock)*m_scene.getObjectCount(), GL_STREAM_DRAW);

	ObjectBlock * ptr = (ObjectBlock*)GPU::mmap(*m_pObjectsBuffer, GL_WRITE_ONLY);

	for (const Object & object : m_scene.getObjects())
	{
		ptr->Model = object.transformation;
		++ptr;
	}

	GPU::munmap(*m_pObjectsBuffer);
}

/**
 * @brief Rendering::onResize
 * @param width
 * @param height
 */
void Rendering::onResize(int width, int height)
{
	m_uWidth        = width;
	m_uHeight       = height;

	//m_matShadowMapProjection = _perspective(45.0f, 1.0f, 1.0f, 100.0f); // FIXME : spot light 45° hardcoded
	m_matShadowMapProjection = _ortho(-20.0f, 20.0f, -20.0f, 20.0f, -10.0f, 100.0f); // FIXME : use scene AABB and/or camera frustrum to compute this

	for (std::map<std::string, RenderTexture>::iterator it = m_mapTargets.begin(); it != m_mapTargets.end(); ++it)
	{
		it->second.resize(m_uWidth, m_uHeight);
	}
}

/**
 * @brief Rendering::onUpdate
 * @param mProjection
 * @param mView
 */
void Rendering::onUpdate(const mat4x4 & mView, const vec4 & clearColor)
{
	rmt_ScopedCPUSample(Update, 0);
	rmt_ScopedOpenGLSample(Update);

	updateCameraBuffer(mView);

	const vec4 clearColorXYZ(RGB_to_XYZ * clearColor.xyz, clearColor.w);
	const vec4 clearColorRGB(XYZ_to_RGB * clearColorXYZ.xyz, clearColor.w);

	m_matCurrentView = mView;
	m_vCurrentClearColor = clearColorXYZ;

	rmt_BeginCPUSample(loop, 0);

	for (GraphicsAlgorithm * a : m_renderQueue)
	{
		rmt_ScopedCPUSample(iteration, 0)

		RHI::CommandBuffer commandBuffer;

		commandBuffer.Begin();

		a->render(commandBuffer);

		commandBuffer.End();
	}

	rmt_EndCPUSample();
}

/**
 * @brief Rendering::onMeshImported
 * @param scene
 * @param MeshID
 * @param vertexData
 */
void Rendering::onMeshImported(const ResourceManager & scene, unsigned int MeshID, const VertexData & vertexData)
{
	// TODO
}

/**
 * @brief Rendering::onMeshImported
 * @param scene
 * @param MeshID
 * @param vertexData
 * @param indexData
 */
void Rendering::onMeshImported(const ResourceManager & scene, unsigned int MeshID, const VertexData & vertexData, const IndexData & indexData)
{
	const uint NumVertices = vertexData.vertexCount;
	const uint NumIndices = indexData.indexCount;

	// Reserve GPU memory for the vertex attributes and indices
	GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();
	GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer = new GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER>();

	GPU::realloc(*vertexBuffer, NumVertices * sizeof(SubMesh::VertexSimple));
	GPU::realloc(*indexBuffer, NumIndices * 3 * sizeof(unsigned int));

	void * pVertexGPU = GPU::mmap(*vertexBuffer, GL_WRITE_ONLY);
	void * pIndexGPU = GPU::mmap(*indexBuffer, GL_WRITE_ONLY);

	std::vector<Mesh::VertexSpec> specs;

	Mesh::VertexSpec SPEC_POS;
	SPEC_POS.index			= 0;
	SPEC_POS.size			= 3;
	SPEC_POS.type			= GL_FLOAT;
	SPEC_POS.normalized		= GL_FALSE;
	SPEC_POS.stride			= sizeof(SubMesh::VertexSimple);
	SPEC_POS.pointer		= BUFFER_OFFSET(0);

	Mesh::VertexSpec SPEC_UV;
	SPEC_UV.index			= 2;
	SPEC_UV.size			= 2;
	SPEC_UV.type			= GL_FLOAT;
	SPEC_UV.normalized		= GL_FALSE;
	SPEC_UV.stride			= sizeof(SubMesh::VertexSimple);
	SPEC_UV.pointer			= BUFFER_OFFSET(sizeof(float)*3);

	Mesh::VertexSpec SPEC_NORMAL;
	SPEC_NORMAL.index		= 1;
	SPEC_NORMAL.size		= 3;
	SPEC_NORMAL.type		= GL_FLOAT;
	SPEC_NORMAL.normalized	= GL_FALSE;
	SPEC_NORMAL.stride		= sizeof(SubMesh::VertexSimple);
	SPEC_NORMAL.pointer		= BUFFER_OFFSET(sizeof(float)*5);

	Mesh::VertexSpec SPEC_TANGENT;
	SPEC_TANGENT.index		= 3;
	SPEC_TANGENT.size		= 3;
	SPEC_TANGENT.type		= GL_FLOAT;
	SPEC_TANGENT.normalized	= GL_FALSE;
	SPEC_TANGENT.stride		= sizeof(SubMesh::VertexSimple);
	SPEC_TANGENT.pointer	= BUFFER_OFFSET(sizeof(float)*8);

	specs.push_back(SPEC_POS);
	specs.push_back(SPEC_UV);
	specs.push_back(SPEC_NORMAL);
	specs.push_back(SPEC_TANGENT);

	Mesh * m = new Mesh(vertexBuffer, specs, indexBuffer);

	//unsigned int vertex_offset = 0;
	unsigned int index_offset = 0;
	unsigned int base_vertex = 0;

	vec3 * Vertices = (vec3*)vertexData.vertices[VertexData::VERTEX_TYPE_POSITION];
	if (Vertices)
	{
		assert(vertexData.vertexFormat[VertexData::VERTEX_TYPE_POSITION] == VERTEX_FORMAT_FLOAT3);
	}

	vec3 * Normals = (vec3*)vertexData.vertices[VertexData::VERTEX_TYPE_NORMAL];
	if (Normals)
	{
		assert(vertexData.vertexFormat[VertexData::VERTEX_TYPE_NORMAL] == VERTEX_FORMAT_FLOAT3);
	}

	vec3 * Tangents = (vec3*)vertexData.vertices[VertexData::VERTEX_TYPE_TANGENT];
	if (Tangents)
	{
		assert(vertexData.vertexFormat[VertexData::VERTEX_TYPE_TANGENT] == VERTEX_FORMAT_FLOAT3);
	}

	vec3 * TextureCoords = (vec3*)vertexData.vertices[VertexData::VERTEX_TYPE_TEX_COORD0];
	if (TextureCoords)
	{
		assert(vertexData.vertexFormat[VertexData::VERTEX_TYPE_TEX_COORD0] == VERTEX_FORMAT_FLOAT3);
	}

	//for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		//aiMesh * mesh = scene->mMeshes[i];

		std::vector<SubMesh::VertexSimple> vertices;
		vertices.reserve(NumVertices);

		// Populate the vertex attribute vectors
		for (int j = 0 ; j < NumVertices ; ++j)
		{
			SubMesh::VertexSimple vertex;

			vertex.position.x = Vertices[j].x;
			vertex.position.y = Vertices[j].y;
			vertex.position.z = Vertices[j].z;

			vertex.normal.x = Normals[j].x;
			vertex.normal.y = Normals[j].y;
			vertex.normal.z = Normals[j].z;

			if (Tangents)
			{
				vertex.tangent.x = Tangents[j].x;
				vertex.tangent.y = Tangents[j].y;
				vertex.tangent.z = Tangents[j].z;
			}

			if (TextureCoords)
			{
				vertex.uv.x = TextureCoords[j].x;
				vertex.uv.y = TextureCoords[j].y;
			}
			else
			{
				vertex.uv.x = 0.5f;
				vertex.uv.y = 0.5f;
			}

			vertices.push_back(vertex);
		}

		memcpy(pVertexGPU, (void *)vertices.data(), vertices.size() * sizeof(SubMesh::VertexSimple));
		memcpy(pIndexGPU, (void *)indexData.indices, indexData.indexCount * sizeof(unsigned int));

		SubMesh * subMesh = m->AddSubMesh(indexData.indexCount, GL_TRIANGLES, index_offset, GL_UNSIGNED_INT, base_vertex);
	}

	GPU::munmap(*vertexBuffer);
	GPU::munmap(*indexBuffer);

	assert(m_aLoadedMeshes.size() == (MeshID-1)); // otherwise we will need a map instead of a simple vector
	m_aLoadedMeshes.push_back(m);
}

/**
 * @brief Rendering::onTextureImported
 * @param scene
 * @param TextureID
 * @param textureData
 */
void Rendering::onTextureImported(const ResourceManager & scene, unsigned int TextureID, const TextureData1D & textureData)
{
	// TODO
}

/**
 * @brief Rendering::onTextureImported
 * @param scene
 * @param TextureID
 * @param textureData
 */
void Rendering::onTextureImported(const ResourceManager & scene, unsigned int TextureID, const TextureData2D & textureData)
{
	GLuint textureId = 0;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	if (textureData.texelFormat == TEXEL_FORMAT_RGBA8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
	}
	else if (textureData.texelFormat == TEXEL_FORMAT_RGB8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureData.width, textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);
	}
	else if (textureData.texelFormat == TEXEL_FORMAT_BGRA8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureData.width, textureData.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData.data);
	}
	else if (textureData.texelFormat == TEXEL_FORMAT_BGR8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureData.width, textureData.height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData.data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	assert(m_aLoadedTextures.size() == (TextureID-1)); // otherwise we will need a map instead of a simple vector
	m_aLoadedTextures.push_back(textureId);
}

/**
 * @brief Rendering::onTextureImported
 * @param scene
 * @param TextureID
 * @param textureData
 */
void Rendering::onTextureImported(const ResourceManager & scene, unsigned int TextureID, const TextureData3D & textureData)
{
	// TODO
}

/**
 * @brief Rendering::onObjectInserted
 * @param object
 */
void Rendering::onObjectInserted(const Scene & scene, const Object & object)
{
	updateObjectsBuffer();
}

/**
 * @brief Rendering::onObjectRemoved
 * @param object
 */
void Rendering::onObjectRemoved(const Scene & scene, const Object & instance)
{
	updateObjectsBuffer();
}

/**
 * @brief Rendering::onCameraInserted
 * @param scene
 * @param camera
 */
void Rendering::onCameraInserted(const Scene & scene, const Camera & camera)
{
	// nothing
}

/**
 * @brief Rendering::onCameraRemoved
 * @param scene
 * @param camera
 */
void Rendering::onCameraRemoved(const Scene & scene, const Camera & camera)
{
	// nothing
}

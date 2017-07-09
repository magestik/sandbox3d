﻿#include "Rendering.h"

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

#define MAX_Z 1000.0f
#define MIN_Z 0.0001f

#define ENABLE_PICKBUFFER 1

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::map<std::string, GLuint> g_Textures;

std::map<std::string, Mesh> g_Meshes;

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
Rendering::Rendering(void)
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
{
	GraphicsAlgorithm::RegisterEverything();
	m_scene.registerListener(this);
}

/**
 * @brief Rendering::onReady
 */
void Rendering::onReady(void)
{
	assert(!m_bReady);

	initShaders();


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

	initPickBuffer();

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
void Rendering::initShaders(void)
{
	DIR * dir = opendir("data/shaders");

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
				std::string strFilePath = std::string("data/shaders/") + ep->d_name;
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
	//
	// compute minZ / maxZ
	{
		m_fMinZ = INFINITY;
		m_fMaxZ = 0.0f;

		for (const Object & object : m_scene.getObjects())
		{
			const mat4x4 mMVP = matView * object.transformation;

			vec4 centerInViewProjected = vec4(object.mesh->m_BoundingSphere.center, 1.0f) * mMVP;
			vec3 centerInView = centerInViewProjected.xyz / centerInViewProjected.w;

			vec4 radiusPointViewProjected = vec4(object.mesh->m_BoundingSphere.center.xy, object.mesh->m_BoundingSphere.center.z + object.mesh->m_BoundingSphere.radius, 1.0f) * mMVP;
			vec3 radiusPointView = radiusPointViewProjected.xyz / radiusPointViewProjected.w;

			float fRadiusInView = distance(centerInView, radiusPointView);

			{
				float maxZ = -centerInView.z + fRadiusInView;

				if (maxZ < MAX_Z)
				{
					if (maxZ > m_fMaxZ)
					{
						m_fMaxZ = maxZ;
					}
				}

				float minZ = -centerInView.z - fRadiusInView;

				if (minZ > MIN_Z)
				{
					if (minZ < m_fMinZ)
					{
						m_fMinZ = minZ;
					}
				}
			}
		}
	}

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

#if ENABLE_PICKBUFFER
	renderPickBuffer();
#endif // ENABLE_PICKBUFFER
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
 * @brief Rendering::initPickBuffer
 */
void Rendering::initPickBuffer(void)
{
	//
	// Create Render Pass
	RHI::RenderPass::SubpassDescription desc;
	desc.depthAttachment = 0; // disable depth buffer
	desc.aColorAttachments.push_back(0);

	m_pickBufferRenderPass = RHI::RenderPass(desc);

	//
	// Create Pipeline
	RHI::PipelineInputAssemblyStateCreateInfo input;
	RHI::PipelineRasterizationStateCreateInfo rasterization;

	RHI::PipelineDepthStencilStateCreateInfo depthStencil;
	depthStencil.enableDepth = true;
	depthStencil.depthState.enableWrite = true;
	depthStencil.depthState.compareOp = RHI::COMPARE_OP_LESS;

	RHI::PipelineBlendStateCreateInfo blend;

	RHI::PipelineShaderStageCreateInfo vertexShader;
	vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
	vertexShader.module = m_mapShaderModules["pickbuffer.vert"];

	RHI::PipelineShaderStageCreateInfo fragmentShader;
	fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
	fragmentShader.module = m_mapShaderModules["pickbuffer.frag"];

	std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
	aStages.push_back(vertexShader);
	aStages.push_back(fragmentShader);

	m_pickBufferPipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pickBufferPipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pickBufferPipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);
}

/**
 * @brief Rendering::renderPickBuffer
 */
void Rendering::renderPickBuffer(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	commandBuffer.BeginRenderPass(m_pickBufferRenderPass, m_pickBufferFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight), vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	{
		commandBuffer.Bind(m_pickBufferPipeline);

		unsigned int i = 0;

		for (const Object & object : m_scene.getObjects())
		{
			object.mesh->bind();

			SetUniform(m_pickBufferPipeline.m_uShaderObject, "Model", object.transformation);

			glVertexAttribI1ui(5, i);

			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++i;
		}
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

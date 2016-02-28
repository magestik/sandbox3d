#include "Rendering.h"

#include <assert.h>

#include "utils.inl"

#include "Pipeline.h"

#include "RenderXML.h"

std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *> g_FragmentShaders;

std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *> g_VertexShaders;

std::map<std::string, GPU::Shader<GL_GEOMETRY_SHADER> *> g_GeometryShaders;

std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

std::map<std::string, Mesh> g_Meshes;

#define SHADOW_MAP_SIZE 1024

const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

static const char PIPELINE_STR []	= "pipeline";
static const char PASS_STR []		= "pass";

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
Rendering::Rendering()
: m_uWidth(1280)
, m_uHeight(720)
, m_pQuadMesh(nullptr)
, m_pCameraBuffer(nullptr)
, m_pObjectsBuffer(nullptr)
, m_mapTargets()
, m_mapPass()
, m_AvLum(nullptr)
{
	// ...
}

/**
 * @brief Rendering::onInitializeComplete
 */
void Rendering::onInitializeComplete()
{
	RenderXML renderXML("data/render.xml");

	renderXML.initializePipelines(*this);
	renderXML.initializeTargets(*this);

	onResize(m_uWidth, m_uHeight);

	renderXML.initializeFramebuffers(*this);
	renderXML.initializePasses(*this);

	//
	// Initialize hardcoded passes
	m_AvLum = new AverageLuminance(m_mapPipeline["average_luminance"]);
	m_AvLum->init(m_mapTargets["luminance1"].getTexture(), m_mapTargets["luminance2"].getTexture());

	m_pLight = new Light::Directionnal(vec3(-20.0f, -20.0f, -20.0f));

	//
	// Generate Meshes
	generateMeshes();

	//
	// Initialize Uniform blocks
	m_pCameraBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
	GPU::realloc(*m_pCameraBuffer, sizeof(CameraBlock), GL_STREAM_DRAW);

	m_pObjectsBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
	GPU::realloc(*m_pObjectsBuffer, sizeof(ObjectBlock)*m_aObjects.size(), GL_STREAM_DRAW);

	for (auto const & p : m_mapPipeline)
	{
		p.second->SetUniformBlockBinding("CameraBlock", BLOCK_BINDING_CAMERA);
		p.second->SetUniformBlockBinding("ObjectBlock", BLOCK_BINDING_OBJECT);
	}

	glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_CAMERA, m_pCameraBuffer->GetObject(), 0, sizeof(CameraBlock));
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

	{
		float points [] =
		{
			0.0f, 0.0f,
		};

		GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer = new GPU::Buffer<GL_ARRAY_BUFFER>();

		GPU::realloc(*vertexBuffer, sizeof(points), GL_STATIC_DRAW, points);

		std::vector<Mesh::VertexSpec> specs;

		Mesh::VertexSpec SPEC_POS;
		SPEC_POS.index = 0;
		SPEC_POS.size = 2;
		SPEC_POS.type = GL_FLOAT;
		SPEC_POS.normalized = GL_FALSE;
		SPEC_POS.stride = 2 * sizeof(float);
		SPEC_POS.pointer = 0;

		specs.push_back(SPEC_POS);

		m_pPointMesh = new Mesh(vertexBuffer, specs);
		m_pPointMesh->AddSubMesh(1, GL_POINTS);
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
	CameraBlock cam;
	cam.ViewProjection = m_matProjection * matView;
	cam.View = matView;

	GPU::memcpy(*m_pCameraBuffer, &cam, sizeof(CameraBlock));
}

/**
 * @brief Rendering::updateObjectsBuffer
 */
void Rendering::updateObjectsBuffer(void)
{
	GPU::realloc(*m_pObjectsBuffer, sizeof(ObjectBlock)*m_aObjects.size(), GL_STREAM_DRAW);

	ObjectBlock * ptr = (ObjectBlock*)GPU::mmap(*m_pObjectsBuffer, GL_WRITE_ONLY);

	for (const Mesh::Instance & o : m_aObjects)
	{
		ptr->Model = o.transformation;
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

	m_uLuminanceSizePOT     = toPOT((width > height) ? width : height);

	float ratio = m_uWidth/(float)m_uHeight;
	m_matProjection = _perspective(75.0f, ratio, 1.0f, 1000.0f);

	//m_matShadowMapProjection = _perspective(45.0f, 1.0f, 1.0f, 100.0f); // FIXME : spot light 45° hardcoded
	m_matShadowMapProjection = _ortho(-20.0f, 20.0f, -20.0f, 20.0f, -10.0f, 100.0f);

	for (std::map<std::string, RenderTexture>::iterator it = m_mapTargets.begin(); it != m_mapTargets.end(); ++it)
	{
		if (it->first == "luminance1" || it->first == "luminance2")
		{
			it->second.resize(m_uLuminanceSizePOT, m_uLuminanceSizePOT);
		}
		else if (it->first == "shadow_map")
		{
			it->second.resize(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		}
		else
		{
			it->second.resize(m_uWidth, m_uHeight);
		}
	}
}

/**
 * @brief Rendering::onUpdate
 * @param mProjection
 * @param mView
 */
void Rendering::onUpdate(const mat4x4 & mView, const vec4 & clearColor, bool bWireframe, ERenderType eRenderType, const Mesh::Instance * pSelectedObject)
{
	updateCameraBuffer(mView);

	const vec4 clearColorXYZ(sRGB_to_XYZ * clearColor.xyz, clearColor.w);
	const vec4 clearColorRGB(XYZ_to_sRGB * clearColorXYZ.xyz, clearColor.w);

	if (FINAL == eRenderType && bWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		renderSceneToGBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		renderLightsToAccumBuffer(mView);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		renderFinal(mView, clearColorXYZ);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		renderIntermediateToScreen(FINAL);

		return;
	}

	//
	// Render Scene to G-Buffer
	//

	renderSceneToGBuffer();

	//
	// Render Scene to Shadow Map
	//

	renderSceneToShadowMap();


	//
	// Render Lights to Accumulation Buffer
	//

	renderLightsToAccumBuffer(mView);

	//
	// Render Scene using light buffer
	//

	renderFinal(mView, clearColorXYZ);

	//
	// Add Fog if requested
	//

	if (environment.isEnabled(EnvironmentSettings::FOG))
	{
		renderFog();
	}

	//
	// Bloom
	//

	renderBloom();

	//
	// Render to Screen
	//

	if (eRenderType == FINAL)
	{
		float avLum = 0.0f;
		float white2 = 0.0f;

		computeToneMappingParams(avLum, white2);

		RHI::CommandBuffer commandBuffer;

		commandBuffer.Begin();

		RHI::Pipeline pipeline;
		RHI::RenderPass & ToneMappingTechnique = m_mapPass["tonemapping"];
		RHI::Framebuffer & ToneMappingFramebuffer = m_mapFramebuffer["LDR"];

		commandBuffer.BeginRenderPass(ToneMappingTechnique, ToneMappingFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
		{
			commandBuffer.Bind(pipeline);

			m_mapPipeline["tonemapping"]->Bind();

			m_mapPipeline["tonemapping"]->SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));
			m_mapPipeline["tonemapping"]->SetUniform("avLum", avLum);
			m_mapPipeline["tonemapping"]->SetUniform("white2", white2);

			m_pQuadMesh->draw(commandBuffer);

			m_mapPipeline["tonemapping"]->UnBind();
		}
		commandBuffer.EndRenderPass();

		RHI::RenderPass & AntiAliasingTechnique = m_mapPass["anti-aliasing"];
		RHI::Framebuffer & AntiAliasingFramebuffer = m_mapFramebuffer["default"];

		commandBuffer.BeginRenderPass(AntiAliasingTechnique, AntiAliasingFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
		{
			commandBuffer.Bind(pipeline);

			m_mapPipeline["fxaa"]->Bind();

			m_mapPipeline["fxaa"]->SetTexture("texSampler", 0, *(m_mapTargets["LDR"].getTexture()));
			m_mapPipeline["fxaa"]->SetUniform("fxaaQualityRcpFrame", vec2(1.0/m_uWidth, 1.0/m_uHeight));

			m_pQuadMesh->draw(commandBuffer);

			m_mapPipeline["fxaa"]->UnBind();
		}
		commandBuffer.EndRenderPass();

		commandBuffer.End();

		//
		// post process

		renderPostProcessEffects();

		renderPickBuffer();

		if (nullptr != pSelectedObject)
		{
			renderBoundingBox(pSelectedObject);
		}
	}
	else
	{
		renderIntermediateToScreen(eRenderType);
	}
}

/**
 * @brief Rendering::onCreate
 * @param m
 */
void Rendering::onCreate(const Mesh::Instance & instance)
{
	m_aObjects.push_back(instance);

	updateObjectsBuffer();
}

/**
 * @brief Rendering::onDelete
 * @param instance
 */
void Rendering::onDelete(const Mesh::Instance & instance)
{
	for (std::vector<Mesh::Instance>::iterator it = m_aObjects.begin() ; it != m_aObjects.end(); ++it)
	{
		const Mesh::Instance & current = *it;

		if (&current == &instance)
		{
			m_aObjects.erase(it);
			break;
		}
	}

	updateObjectsBuffer();
}

/**
 * @brief Rendering::getObjectAtPos
 * @param pos
 * @return
 */
Mesh::Instance * Rendering::getObjectAtPos(const ivec2 & pos)
{
	ivec2 glPos(pos.x, m_uHeight - pos.y);

	Mesh::Instance * object = nullptr;

	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & PickBufferTechnique = m_mapPass["picking"];
	RHI::Framebuffer & PickBufferFramebuffer = m_mapFramebuffer["pickbuffer-earlyZ"];

	commandBuffer.BeginRenderPass(PickBufferTechnique, PickBufferFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
	{
		unsigned int id = UINT32_MAX;
#if 0
		PickBufferTechnique.ReadPixel(glPos, id);
#endif
		if (id < m_aObjects.size())
		{
			object = &(m_aObjects[id]);
		}
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();

	return(object);
}

/**
 * @brief Rendering::renderSceneToShadowMap
 * @param mViewProjection
 */
void Rendering::renderSceneToShadowMap(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & ShadowMapPass = m_mapPass["shadow_map"];
	RHI::Framebuffer & ShadowMapFramebuffer = m_mapFramebuffer["shadow-map"];

	commandBuffer.BeginRenderPass(ShadowMapPass, ShadowMapFramebuffer, ivec2(0, 0), ivec2(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE), 1.0f, 0);
	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["shadow_map"]->Bind();

		glPolygonOffset(10.0f, 1.0f);

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_matShadowMapProjection * mDepthView;

		m_mapPipeline["shadow_map"]->SetUniform("LightViewProjection", mDepthViewProjection);

		for (Mesh::Instance & object : m_aObjects)
		{
			m_mapPipeline["shadow_map"]->SetUniform("Model", object.transformation);

			object.mesh->bind();

			// TODO : remove loop and directly use glDrawElements on the full buffer
			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw(commandBuffer);
			}

			object.mesh->unbind();
		}

		m_mapPipeline["shadow_map"]->UnBind();
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderIntermediateToScreen
 * @param mViewProjection
 */
void Rendering::renderIntermediateToScreen(ERenderType eRenderType)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Framebuffer & DefaultFramebuffer = m_mapFramebuffer["default"];

	switch (eRenderType)
	{
		case FINAL:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_color"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_color"]->Bind();

				m_mapPipeline["debug_color"]->SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_color"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;

		case DIFFUSE_LIGHTS:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_color"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_color"]->Bind();

				m_mapPipeline["debug_color"]->SetTexture("texSampler", 0, *(m_mapTargets["lights_diffuse"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_color"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;

		case SPECULAR_LIGHTS:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_color"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_color"]->Bind();

				m_mapPipeline["debug_color"]->SetTexture("texSampler", 0, *(m_mapTargets["lights_specular"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_color"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;

		case NORMAL_BUFFER:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_normals"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_normal"]->Bind();

				m_mapPipeline["debug_normal"]->SetTexture("texSampler", 0, *(m_mapTargets["normals"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_normal"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;

		case DEPTH:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_depth"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_depth"]->Bind();

				m_mapPipeline["debug_depth"]->SetTexture("texSampler", 0, *(m_mapTargets["depth"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_depth"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;

		case SHADOWS:
		{
			RHI::Pipeline pipeline;

			RHI::RenderPass & DebugPass = m_mapPass["debug_depth"];

			commandBuffer.BeginRenderPass(DebugPass, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
			{
				commandBuffer.Bind(pipeline);

				m_mapPipeline["debug_depth"]->Bind();

				m_mapPipeline["debug_depth"]->SetTexture("texSampler", 0, *(m_mapTargets["shadow_map"].getTexture()));

				m_pQuadMesh->draw(commandBuffer);

				m_mapPipeline["debug_depth"]->UnBind();
			}
			commandBuffer.EndRenderPass();
		}
		break;
/*
		case LUMINANCE1:
		{
			DebugTechnique.BeginPass("luminance");
			{
				DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["luminance1"].getTexture()));
				m_pQuadMesh->draw();
			}
			DebugTechnique.EndPass();
		}
		break;

		case LUMINANCE2:
		{
			DebugTechnique.BeginPass("luminance");
			{
				DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["luminance2"].getTexture()));
				m_pQuadMesh->draw();
			}
			DebugTechnique.EndPass();
		}
		break;

		case BLOOM:
		{
			DebugTechnique.BeginPass("color");
			{
				DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["bloom1"].getTexture()));
				m_pQuadMesh->draw();
			}
			DebugTechnique.EndPass();
		}
		break;
*/
	}

	commandBuffer.End();
}

/**
 * @brief Rendering::renderGBuffer
 */
void Rendering::renderSceneToGBuffer(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & GeometryPass = m_mapPass["geometry"];
	RHI::Framebuffer & GeometryFramebuffer = m_mapFramebuffer["normals-earlyZ"];

	commandBuffer.BeginRenderPass(GeometryPass, GeometryFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight), vec4(0.0f ,0.0f, 0.0f, 0.0f), 1.0f, 0);

	{
		commandBuffer.Bind(pipeline);

		// OPTIMIZE THIS !!!!!
		{
			m_mapPipeline["geometry_simple"]->Bind();

			unsigned int offset = 0;

			for (Mesh::Instance & object : m_aObjects)
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_OBJECT, m_pObjectsBuffer->GetObject(), sizeof(ObjectBlock)*offset, sizeof(ObjectBlock));

				object.mesh->bind();

				for (SubMesh * m : object.getDrawCommands())
				{
					const GPU::Texture<GL_TEXTURE_2D> * pNormalMap = m->getNormalMap();

					if (nullptr == pNormalMap)
					{
						m_mapPipeline["geometry_simple"]->SetUniform("shininess", m->m_material.shininess);

						m->draw(commandBuffer);
					}
				}

				object.mesh->unbind();

				++offset;
			}

			m_mapPipeline["geometry_simple"]->UnBind();
		}

		commandBuffer.NextSubpass();

		{
			m_mapPipeline["geometry_normalmap"]->Bind();

			unsigned int offset = 0;

			for (Mesh::Instance & object : m_aObjects)
			{
				glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_OBJECT, m_pObjectsBuffer->GetObject(), sizeof(ObjectBlock)*offset, sizeof(ObjectBlock));

				object.mesh->bind();

				for (SubMesh * m : object.getDrawCommands())
				{
					const GPU::Texture<GL_TEXTURE_2D> * pNormalMap = m->getNormalMap();

					if (nullptr != pNormalMap)
					{
						m_mapPipeline["geometry_normalmap"]->SetTexture("normalMap", 0, *pNormalMap);
						m_mapPipeline["geometry_normalmap"]->SetUniform("shininess", m->m_material.shininess);

						m->draw(commandBuffer);
					}
				}

				object.mesh->unbind();

				++offset;
			}

			m_mapPipeline["geometry_normalmap"]->UnBind();
		}
	}

	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderLightsToAccumBuffer
 */
void Rendering::renderLightsToAccumBuffer(const mat4x4 & mView)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & LightsTechnique = m_mapPass["lights"];
	RHI::Framebuffer & LightsFramebuffer = m_mapFramebuffer["lights"];

	commandBuffer.BeginRenderPass(LightsTechnique, LightsFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight), vec4(0.0f, 0.0f, 0.0f, 0.0f));

	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["lights_directional"]->Bind();

		mat4x4 mCameraViewProjection = m_matProjection * mView;

		m_mapPipeline["lights_directional"]->SetUniform("viewPos", (inverse(mView) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
		m_mapPipeline["lights_directional"]->SetUniform("InverseViewProjection", inverse(mCameraViewProjection));
		m_mapPipeline["lights_directional"]->SetUniform("lightDir", - normalize(m_pLight->GetDirection()));
		m_mapPipeline["lights_directional"]->SetUniform("lightColor", sRGB_to_XYZ * m_pLight->GetColor());
		m_mapPipeline["lights_directional"]->SetTexture("depthSampler", 0, *(m_mapTargets["depth"].getTexture()));
		m_mapPipeline["lights_directional"]->SetTexture("normalSampler", 1, *(m_mapTargets["normals"].getTexture()));

		m_pQuadMesh->draw(commandBuffer);

		m_mapPipeline["lights_directional"]->UnBind();

		// TODO : render all lights
	}

	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderFinal
 * @param mViewProjection
 */
void Rendering::renderFinal(const mat4x4 & mView, const vec4 & clearColor)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & ComposeTechnique = m_mapPass["compose"];
	RHI::Framebuffer & ComposeFramebuffer = m_mapFramebuffer["HDR-earlyZ"];

	commandBuffer.BeginRenderPass(ComposeTechnique, ComposeFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight), vec4(clearColor.x, clearColor.y, clearColor.z, clearColor.w));

	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["compose"]->Bind();

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_matShadowMapProjection * mDepthView;

		m_mapPipeline["compose"]->SetTexture("diffuseLightSampler", 0, *(m_mapTargets["lights_diffuse"].getTexture()));
		m_mapPipeline["compose"]->SetTexture("specularLightSampler", 1, *(m_mapTargets["lights_specular"].getTexture()));
		m_mapPipeline["compose"]->SetTexture("shadowMap", 2, *(m_mapTargets["shadow_map"].getTexture()));

		m_mapPipeline["compose"]->SetUniform("ambientColor", sRGB_to_XYZ * environment.ambient.Color);
		m_mapPipeline["compose"]->SetUniform("DepthTransformation", mDepthViewProjection);
		m_mapPipeline["compose"]->SetUniform("View", mView);

		unsigned int offset = 0;

		for (Mesh::Instance & object : m_aObjects)
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_OBJECT, m_pObjectsBuffer->GetObject(), sizeof(ObjectBlock)*offset, sizeof(ObjectBlock));

			object.mesh->bind();

			for (SubMesh * m : object.getDrawCommands())
			{
				m_mapPipeline["compose"]->SetTexture("diffuseSampler", 3, *(m->m_material.m_diffuse));
				m_mapPipeline["compose"]->SetTexture("specularSampler", 4, *(m->m_material.m_specular));

				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++offset;
		}

		m_mapPipeline["compose"]->UnBind();
	}

	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderFog
 * @param mView
 */
void Rendering::renderFog(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & ComposeTechnique = m_mapPass["fog"];
	RHI::Framebuffer & ComposeFramebuffer = m_mapFramebuffer["HDR"];

	commandBuffer.BeginRenderPass(ComposeTechnique, ComposeFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));

	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["fog_simple"]->Bind();

		m_mapPipeline["fog_simple"]->SetTexture("depthMapSampler", 0, *(m_mapTargets["depth"].getTexture()));
		m_mapPipeline["fog_simple"]->SetUniform("FogScattering", environment.fog.Scattering);
		m_mapPipeline["fog_simple"]->SetUniform("FogExtinction", environment.fog.Extinction);
		m_mapPipeline["fog_simple"]->SetUniform("FogColor", environment.fog.Color);
		m_mapPipeline["fog_simple"]->SetUniform("camera_near", 1.0f);
		m_mapPipeline["fog_simple"]->SetUniform("camera_far", 1000.0f);
		m_mapPipeline["fog_simple"]->SetUniform("near_plane_half_size", vec2(m_uHeight * float(m_uWidth/(float)m_uHeight), tanf(75.0f/2.0)));

		m_pQuadMesh->draw(commandBuffer);

		m_mapPipeline["fog_simple"]->UnBind();
	}

	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderBloom
 */
void Rendering::renderBloom(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & BloomTechnique = m_mapPass["bloom"];
	RHI::Framebuffer & BloomFramebuffer = m_mapFramebuffer["bloom"];

	commandBuffer.BeginRenderPass(BloomTechnique, BloomFramebuffer, ivec2(0, 0), ivec2(m_uWidth/4, m_uHeight/4));
	{
		commandBuffer.Bind(pipeline);

		{
			m_mapPipeline["bloom_bright"]->Bind();

			m_mapPipeline["bloom_bright"]->SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));

			m_pQuadMesh->draw(commandBuffer);

			m_mapPipeline["bloom_bright"]->UnBind();
		}

		commandBuffer.NextSubpass();

		{
			m_mapPipeline["bloom_horizontal_blur"]->Bind();

			m_mapPipeline["bloom_horizontal_blur"]->SetTexture("texSampler", 0, *(m_mapTargets["bloom1"].getTexture()));

			m_pQuadMesh->draw(commandBuffer);

			m_mapPipeline["bloom_horizontal_blur"]->UnBind();
		}

		commandBuffer.NextSubpass();

		{
			m_mapPipeline["bloom_vertical_blur"]->Bind();

			m_mapPipeline["bloom_vertical_blur"]->SetTexture("texSampler", 0, *(m_mapTargets["bloom2"].getTexture()));

			m_pQuadMesh->draw(commandBuffer);

			m_mapPipeline["bloom_vertical_blur"]->UnBind();
		}
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderPostProcessEffects
 */
void Rendering::renderPostProcessEffects()
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & BlendTechnique = m_mapPass["blend"];
	RHI::Framebuffer & BlendFramebuffer = m_mapFramebuffer["default"];

	commandBuffer.BeginRenderPass(BlendTechnique, BlendFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["bloom"]->Bind();

		m_mapPipeline["bloom"]->SetTexture("texSampler", 0, *(m_mapTargets["bloom1"].getTexture()));

		m_pQuadMesh->draw(commandBuffer);

		m_mapPipeline["bloom"]->UnBind();
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderPickBuffer
 */
void Rendering::renderPickBuffer(void)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & PickBufferTechnique = m_mapPass["picking"];
	RHI::Framebuffer & PickBufferFramebuffer = m_mapFramebuffer["pickbuffer-earlyZ"];

	commandBuffer.BeginRenderPass(PickBufferTechnique, PickBufferFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["pickbuffer"]->Bind();

		unsigned int i = 0;

		for (Mesh::Instance & object : m_aObjects)
		{
			object.mesh->bind();

			m_mapPipeline["pickbuffer"]->SetUniform("Model", object.transformation);

			glVertexAttribI1ui(5, i);

			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++i;
		}

		m_mapPipeline["pickbuffer"]->UnBind();
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::renderBoundingBox
 * @param mView
 */
void Rendering::renderBoundingBox(const Mesh::Instance * pSelectedObject)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	RHI::Pipeline pipeline;
	RHI::RenderPass & BBoxTechnique = m_mapPass["bbox"];
	RHI::Framebuffer & DefaultFramebuffer = m_mapFramebuffer["default"];

	commandBuffer.BeginRenderPass(BBoxTechnique, DefaultFramebuffer, ivec2(0, 0), ivec2(m_uWidth, m_uHeight));
	{
		commandBuffer.Bind(pipeline);

		m_mapPipeline["bbox"]->Bind();

		m_mapPipeline["bbox"]->SetUniform("Model", pSelectedObject->transformation);

		m_mapPipeline["bbox"]->SetUniform("BBoxMin", pSelectedObject->mesh->m_BoundingBox.min);
		m_mapPipeline["bbox"]->SetUniform("BBoxMax", pSelectedObject->mesh->m_BoundingBox.max);
		m_mapPipeline["bbox"]->SetUniform("color", vec3(1.0, 1.0, 1.0));

		m_pPointMesh->draw(commandBuffer);

		m_mapPipeline["bbox"]->UnBind();
	}
	commandBuffer.EndRenderPass();

	commandBuffer.End();
}

/**
 * @brief Rendering::computeLuminance
 * @param avLum
 * @param white²
 */
void Rendering::computeToneMappingParams(float & avLum, float & white2)
{
#if 0
	RHI::RenderPass & ToneMappingComputePass = m_mapPass["compute-tonemapping-params"];
	RHI::Framebuffer & LuminanceFramebuffer = m_mapFramebuffer["luminance"];

	ToneMappingComputePass.BeginRenderPass(LuminanceFramebuffer, ivec2(0, 0), ivec2(m_uLuminanceSizePOT, m_uLuminanceSizePOT));
	{
		{
			m_mapPipeline["luminance_conversion"]->Bind();

			m_mapPipeline["luminance_conversion"]->SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));

			m_pQuadMesh->draw();

			m_mapPipeline["luminance_conversion"]->UnBind();
		}

		const GPU::Texture<GL_TEXTURE_2D> * textures [2] =
		{
			m_mapTargets["luminance1"].getTexture(),
			m_mapTargets["luminance2"].getTexture()
		};

		m_AvLum->begin();
		{
			vec2 texture_scale (1.0f, 1.0f);

			for (int size = m_uLuminanceSizePOT >> 1; size > 1; size >>= 1)
			{
				glViewport(0, 0, size, size);

				unsigned int tex = m_AvLum->next();

				m_AvLum->SetTexture("texSampler", 0, *(textures[tex]));
				m_AvLum->SetUniform("textureScale", texture_scale);

				m_pQuadMesh->draw();

				texture_scale = texture_scale * 0.5f;
			}
		}
		m_AvLum->end();

	}
	ToneMappingComputePass.EndRenderPass();


	avLum = m_AvLum->getAverage();
	white2 = m_AvLum->getMax2();
#else
	avLum = 0.5f;
	white2 = 0.5f;
#endif
}

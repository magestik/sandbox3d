#include "Rendering.h"

#include <assert.h>

#include "utils.inl"

std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *> g_FragmentShaders;

std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *> g_VertexShaders;

std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

std::map<std::string, Mesh> g_Meshes;

#define SHADOW_MAP_SIZE 1024

/**
 * @brief Rendering::onInitialize
 */
Rendering::Rendering()
: m_uWidth(1280)
, m_uHeight(720)
, m_pShadowMap(nullptr)
, m_GBuffer()
, m_LightAccumBuffer()
, m_Compose()
, m_pQuadMesh(nullptr)
{
	// ...
}

void Rendering::onInitializeComplete()
{
	m_apTargets[TARGET_DEPTH]	= new GPU::Texture<GL_TEXTURE_2D>();
	m_apTargets[TARGET_NORMALS]	= new GPU::Texture<GL_TEXTURE_2D>();

	m_apTargets[TARGET_DIFFUSE_LIGHTS]	= new GPU::Texture<GL_TEXTURE_2D>();
	m_apTargets[TARGET_SPECULAR_LIGHTS]	= new GPU::Texture<GL_TEXTURE_2D>();

	m_apTargets[TARGET_POSTFX1]	= new GPU::Texture<GL_TEXTURE_2D>();
	m_apTargets[TARGET_POSTFX2]	= new GPU::Texture<GL_TEXTURE_2D>();

	onResize(1280, 720);

	m_GBuffer.init(m_apTargets[TARGET_NORMALS], m_apTargets[TARGET_DEPTH]);
	m_Compose.init(m_apTargets[TARGET_POSTFX1], m_apTargets[TARGET_DEPTH]);
	m_LightAccumBuffer.init(m_apTargets[TARGET_DIFFUSE_LIGHTS], m_apTargets[TARGET_SPECULAR_LIGHTS]);

	{
		m_pLight = new Light::Directionnal(vec3(-20.0f, -20.0f, -20.0f));
		m_pShadowMap = new ShadowMap();
		m_pShadowMap->init(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	}

	compileShaders();
	generateMeshes();

	glGenSamplers(1,  &m_uSampler);
	glSamplerParameteri(m_uSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_uSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * @brief Compile all needed shaders
 */
void Rendering::compileShaders()
{
	m_pDirectionnalLightShader	= new Shader(g_VertexShaders["directionnal_light.vert"], g_FragmentShaders["directionnal_light.frag"]);

	m_pComposeShader			= new Shader(g_VertexShaders["full.vert"], g_FragmentShaders["full.frag"]);

	m_pGeometryPassShader				= new Shader(g_VertexShaders["geometry_pass.vert"], g_FragmentShaders["geometry_pass.frag"]);
	m_pGeometryWithNormalMapPassShader	= new Shader(g_VertexShaders["geometry_normalmap_pass.vert"], g_FragmentShaders["geometry_normalmap_pass.frag"]);

	m_pDepthOnlyPassShader		= new Shader(g_VertexShaders["depth_only.vert"], g_FragmentShaders["depth_only.frag"]);

	m_pFullscreenDepthShader	= new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_depth.frag"]);
	m_pFullscreenNormalShader	= new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_normal.frag"]);
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

		std::vector<SubMesh::VertexSpec> specs;

		SubMesh::VertexSpec SPEC_POS;
		SPEC_POS.index = 0;
		SPEC_POS.size = 2;
		SPEC_POS.type = GL_FLOAT;
		SPEC_POS.normalized = GL_FALSE;
		SPEC_POS.stride = 4 * sizeof(float);
		SPEC_POS.pointer = 0;

		SubMesh::VertexSpec SPEC_UV;
		SPEC_UV.index = 2;
		SPEC_UV.size = 2;
		SPEC_UV.type = GL_FLOAT;
		SPEC_UV.normalized = GL_FALSE;
		SPEC_UV.stride = 4 * sizeof(float);
		SPEC_UV.pointer = (void*)(sizeof(float)*2);

		specs.push_back(SPEC_POS);
		specs.push_back(SPEC_UV);

		m_pQuadMesh = SubMesh::Create(vertexBuffer, 4, GL_TRIANGLE_STRIP, specs);
	}

	// TODO : Sphere / Cone / Cube
}

/**
 * @brief Rendering::onResize
 * @param width
 * @param height
 */
void Rendering::onResize(int width, int height)
{
	m_apTargets[TARGET_DEPTH  ]->init<GL_DEPTH_COMPONENT32F>(width, height);
	m_apTargets[TARGET_NORMALS]->init<GL_RGBA16F>(width, height);

	m_apTargets[TARGET_DIFFUSE_LIGHTS ]->init<GL_RGB16F>(width, height);
	m_apTargets[TARGET_SPECULAR_LIGHTS]->init<GL_RGB16F>(width, height);

	m_apTargets[TARGET_POSTFX1]->init<GL_RGB16F>(width, height);
	m_apTargets[TARGET_POSTFX2]->init<GL_RGB16F>(width, height);

	m_uWidth = width;
	m_uHeight = height;

	float ratio = m_uWidth/(float)m_uHeight;
	m_matProjection = _perspective(75.0f, ratio, 1.0f, 1000.0f);
}

/**
 * @brief Rendering::onUpdate
 * @param mProjection
 * @param mView
 */
void Rendering::onUpdate(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor, bool bWireframe, ERenderType eRenderType)
{
	//
	// Render Scene to G-Buffer
	//

	renderSceneToGBuffer(mView);

	//
	// Render Scene to Shadow Map
	//

	renderSceneToShadowMap();

	//
	// Render Lights to Accumulation Buffer
	//

	renderLightsToAccumBuffer(mView);

	//
	// Render G-Buffer to Screen
	//

	if (FINAL != eRenderType)
	{
		renderIntermediateToScreen(eRenderType);
	}
	else
	{
		if (bWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		renderFinal(mView, clearColor, ambientColor);

		if (bWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Compose.GetObject());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
}

/**
 * @brief Rendering::onCreate
 * @param m
 */
void Rendering::onCreate(const Mesh::Instance & instance)
{
	m_aObjects.push_back(instance);
}

/**
 * @brief Rendering::renderSceneToShadowMap
 * @param mViewProjection
 */
void Rendering::renderSceneToShadowMap(void)
{
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

	m_pShadowMap->begin();

	{
		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

		m_pDepthOnlyPassShader->SetAsCurrent();

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mDepthViewProjection * object.transformation;

			m_pDepthOnlyPassShader->SetUniform("ModelViewProjection", MVP);

			// TODO : remove loop and directly use glDrawElements on the full buffer
			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw();
			}
		}

		glUseProgram(0);
	}

	m_pShadowMap->end();
}

/**
 * @brief Rendering::renderIntermediateToScreen
 * @param mViewProjection
 */
void Rendering::renderIntermediateToScreen(ERenderType eRenderType)
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	switch (eRenderType)
	{
		case FINAL:
		{
			assert(0);
		}
		break;

		case LIGHT_BUFFER:
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_LightAccumBuffer.GetObject());
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
		break;

		case NORMAL_BUFFER:
		{
			m_pFullscreenNormalShader->SetAsCurrent();
			{
				m_pFullscreenDepthShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_NORMALS]));
				m_pQuadMesh->draw();
			}
			glUseProgram(0);
		}
		break;

		case DEPTH:
		{
			m_pFullscreenDepthShader->SetAsCurrent();
			{
				m_pFullscreenDepthShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_DEPTH]));
				m_pQuadMesh->draw();
			}
			glUseProgram(0);
		}
		break;

		case SHADOWS:
		{
			m_pFullscreenDepthShader->SetAsCurrent();
			{
				m_pFullscreenDepthShader->SetTexture("texSampler", 0, m_pShadowMap->GetTexture());
				m_pQuadMesh->draw();
			}
			glUseProgram(0);
		}
		break;
	}
}

/**
 * @brief Rendering::renderGBuffer
 */
void Rendering::renderSceneToGBuffer(const mat4x4 & mView)
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	m_GBuffer.begin();

	{
		mat4x4 mCameraViewProjection = m_matProjection * mView;

		// OPTIMIZE THIS !!!!!

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mCameraViewProjection * object.transformation;

			for (SubMesh * m : object.getDrawCommands())
			{
				const GPU::Texture<GL_TEXTURE_2D> * pNormalMap = m->getNormalMap();

				if (nullptr != pNormalMap)
				{
					m_pGeometryWithNormalMapPassShader->SetAsCurrent();

					m_pGeometryWithNormalMapPassShader->SetUniform("shininess", m->m_material.shininess);
					m_pGeometryWithNormalMapPassShader->SetUniform("ModelViewProjection", MVP);
					m_pGeometryWithNormalMapPassShader->SetUniform("Model", object.transformation);

					m_pGeometryWithNormalMapPassShader->SetTexture("normalMap", 0, *pNormalMap);

					m->draw();

					glUseProgram(0);
				}
				else
				{
					m_pGeometryPassShader->SetAsCurrent();

					m_pGeometryPassShader->SetUniform("shininess", m->m_material.shininess);
					m_pGeometryPassShader->SetUniform("ModelViewProjection", MVP);
					m_pGeometryPassShader->SetUniform("Model", object.transformation);

					m->draw();

					glUseProgram(0);
				}
			}
		}
	}

	m_GBuffer.end();
}

/**
 * @brief Rendering::renderLightsToAccumBuffer
 */
void Rendering::renderLightsToAccumBuffer(const mat4x4 & mView)
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	m_LightAccumBuffer.begin();

	{
		mat4x4 mCameraViewProjection = m_matProjection * mView;

		m_pDirectionnalLightShader->SetAsCurrent();

		glBindSampler(1, m_uSampler);

		{
			m_pDirectionnalLightShader->SetUniform("viewPos", (inverse(mView) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
			m_pDirectionnalLightShader->SetUniform("InverseViewProjection", inverse(mCameraViewProjection));
			m_pDirectionnalLightShader->SetUniform("lightDir", - normalize(m_pLight->GetDirection()));
			m_pDirectionnalLightShader->SetUniform("lightColor", m_pLight->GetColor());
			m_pDirectionnalLightShader->SetTexture("depthSampler", 0, *(m_apTargets[TARGET_DEPTH]));
			m_pDirectionnalLightShader->SetTexture("normalSampler", 1, *(m_apTargets[TARGET_NORMALS]));
			m_pQuadMesh->draw();
		}

		// TODO : render all lights

		glBindSampler(1, 0);

		glUseProgram(0);
	}

	m_LightAccumBuffer.end();
}

/**
 * @brief Rendering::renderFinal
 * @param mViewProjection
 */
void Rendering::renderFinal(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor)
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	m_Compose.begin(clearColor);

	{
		mat4x4 mCameraViewProjection = m_matProjection * mView;

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

		glBindSampler(3, m_uSampler);
		glBindSampler(4, m_uSampler);

		m_pComposeShader->SetAsCurrent();

		m_pComposeShader->SetTexture("diffuseLightSampler", 0, *(m_apTargets[TARGET_DIFFUSE_LIGHTS]));
		m_pComposeShader->SetTexture("specularLightSampler", 1, *(m_apTargets[TARGET_SPECULAR_LIGHTS]));
		m_pComposeShader->SetTexture("shadowMap", 2, m_pShadowMap->GetTexture());

		m_pComposeShader->SetUniform("ambientColor", ambientColor.xyz);
		m_pComposeShader->SetUniform("DepthTransformation", mDepthViewProjection);

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mCameraViewProjection * object.transformation;

			m_pComposeShader->SetUniform("ModelViewProjection", MVP);
			m_pComposeShader->SetUniform("Model", object.transformation);

			for (SubMesh * m : object.getDrawCommands())
			{
				m_pComposeShader->SetTexture("diffuseSampler", 3, *(m->m_material.m_diffuse));
				m_pComposeShader->SetTexture("specularSampler", 4, *(m->m_material.m_specular));

				m->draw();
			}
		}

		glUseProgram(0);

		glBindSampler(3, 0);
		glBindSampler(4, 0);
	}

	m_Compose.end();
}

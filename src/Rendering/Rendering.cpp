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
	m_apTargets[TARGET_LIGHTS]	= new GPU::Texture<GL_TEXTURE_2D>();
	m_apTargets[TARGET_POSTFX1]	= new GPU::Texture<GL_TEXTURE_2D>();
	m_apTargets[TARGET_POSTFX2]	= new GPU::Texture<GL_TEXTURE_2D>();

	onResize(1280, 720);

	m_GBuffer.init(m_apTargets[TARGET_NORMALS], m_apTargets[TARGET_DEPTH]);
	m_Compose.init(m_apTargets[TARGET_POSTFX1], m_apTargets[TARGET_DEPTH]);
	m_LightAccumBuffer.init(m_apTargets[TARGET_LIGHTS]);

	{
		vec3 light_pos(20.0f, 20.0f, 20.0f);
		vec3 light_dir = - light_pos;
		float light_angle = 75.0f;

		m_pLight = new Light::Spot(light_pos, light_dir, light_angle);
		m_pShadowMap = new ShadowMap();
		m_pShadowMap->init(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	}

	compileShaders();
	generateMeshes();
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
	m_apTargets[TARGET_NORMALS]->init<GL_RGB16F>(width, height);
	m_apTargets[TARGET_LIGHTS ]->init<GL_RGB16F>(width, height);
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
void Rendering::onUpdate(const mat4x4 & mView, const vec4 & clearColor, bool bWireframe, ERenderType eRenderType)
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

	renderLightsToAccumBuffer();

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

		renderFinal(mView, clearColor);

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
void Rendering::onCreate(const Mesh & m)
{
	m_aObjects.push_back(m.Instantiate());
}

/**
 * @brief Rendering::renderSceneToShadowMap
 * @param mViewProjection
 */
void Rendering::renderSceneToShadowMap(void)
{
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

	m_pShadowMap->enable();

	{
		glClear(GL_DEPTH_BUFFER_BIT);

		mat4x4 mDepthView = _lookAt(m_pLight->GetPosition(), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
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

	m_pShadowMap->disable();
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
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
		break;

		case NORMAL_BUFFER:
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer.GetObject());
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
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

	m_GBuffer.enable();

	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

					m_pGeometryWithNormalMapPassShader->SetUniform("ModelViewProjection", MVP);
					m_pGeometryWithNormalMapPassShader->SetUniform("Model", object.transformation);

					m_pGeometryWithNormalMapPassShader->SetTexture("normalMap", 0, *pNormalMap);

					m->draw();

					glUseProgram(0);
				}
				else
				{
					m_pGeometryPassShader->SetAsCurrent();

					m_pGeometryPassShader->SetUniform("ModelViewProjection", MVP);
					m_pGeometryPassShader->SetUniform("Model", object.transformation);

					m->draw();

					glUseProgram(0);
				}
			}
		}
	}

	m_GBuffer.disable();
}

/**
 * @brief Rendering::renderLightsToAccumBuffer
 */
void Rendering::renderLightsToAccumBuffer()
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	m_LightAccumBuffer.enable();

	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_pDirectionnalLightShader->SetAsCurrent();

		{
			m_pDirectionnalLightShader->SetUniform("lightDir", m_pLight->GetDirection());
			m_pDirectionnalLightShader->SetUniform("lightColor", m_pLight->GetColor());
			m_pDirectionnalLightShader->SetTexture("normalSampler", 0, *(m_apTargets[TARGET_NORMALS]));
			m_pQuadMesh->draw();
		}

		// TODO : render all lights

		glUseProgram(0);
	}

	m_LightAccumBuffer.disable();
}

/**
 * @brief Rendering::renderFinal
 * @param mViewProjection
 */
void Rendering::renderFinal(const mat4x4 & mView, const vec4 & clearColor)
{
	glViewport(0, 0, m_uWidth, m_uHeight);

	m_Compose.enable();

	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		mat4x4 mCameraViewProjection = m_matProjection * mView;

		mat4x4 mDepthView = _lookAt(m_pLight->GetPosition(), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

		m_pComposeShader->SetAsCurrent();

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mCameraViewProjection * object.transformation;

			m_pComposeShader->SetUniform("ModelViewProjection", MVP);
			m_pComposeShader->SetUniform("Model", object.transformation);

			m_pComposeShader->SetTexture("lightSampler", 0, *(m_apTargets[TARGET_LIGHTS]));

			m_pComposeShader->SetUniform("DepthTransformation", mDepthViewProjection);
			m_pComposeShader->SetTexture("shadowMap",	3, m_pShadowMap->GetTexture());

			for (SubMesh * m : object.getDrawCommands())
			{
				m->draw();
			}
		}

		glUseProgram(0);
	}

	m_Compose.disable();
}

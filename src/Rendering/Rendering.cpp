#include "Rendering.h"

#include <assert.h>

#include "utils.inl"

#define LIGHT_PRE_PASS 0

std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *> g_FragmentShaders;

std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *> g_VertexShaders;

std::map<std::string, Mesh*> g_Meshes;

/**
 * @brief Rendering::onInitialize
 */
Rendering::Rendering()
: m_pGBuffer(nullptr)
, m_pShadowMap(nullptr)
, m_pQuadMesh(nullptr)
{
	// ...
}

void Rendering::onInitializeComplete()
{
	m_pGBuffer		= new GBuffer();
	m_pShadowMap	= new ShadowMap();

	m_lightAccumBuffer.Initialize();

	{
		vec3 light_pos(20.0f, 20.0f, 20.0f);
		vec3 light_dir = - light_pos;
		float light_angle = 75.0f;

		m_pLight = new Light::Spot(light_pos, light_dir, light_angle);
	}

	compileShaders();
	generateMeshes();
}

/**
 * @brief Compile all needed shaders
 */
void Rendering::compileShaders()
{
	m_pGeometryPassShader		= new Shader(g_VertexShaders["geometry_pass.vs"], g_FragmentShaders["geometry_pass.fs"]);
	m_pLightPassShader			= nullptr;
	m_pDepthOnlyPassShader		= new Shader(g_VertexShaders["depth_only.vs"], g_FragmentShaders["depth_only.fs"]);
	m_pFullscreenDepthShader	= new Shader(g_VertexShaders["fullscreen.vs"], g_FragmentShaders["fullscreen_depth.fs"]);
	m_pFullscreenNormalShader	= new Shader(g_VertexShaders["fullscreen.vs"], g_FragmentShaders["fullscreen_normal.fs"]);
	m_pFullscreenComposeShader	= new Shader(g_VertexShaders["fullscreen.vs"], g_FragmentShaders["compose.fs"]);
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

		m_pQuadMesh = Mesh::Create(vertexBuffer, 4, GL_TRIANGLE_STRIP, specs);
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
	m_pGBuffer->Resize(width, height);
	m_lightAccumBuffer.Resize(width, height);
}

/**
 * @brief Rendering::onUpdate
 * @param mProjection
 * @param mView
 */
void Rendering::onUpdate(const mat4x4 & mView, bool bWireframe, ERenderType eRenderType)
{
	//
	// Render Scene to Shadow Map
	//

	renderSceneToShadowMap();

	//
	// Render Scene to G-Buffer
	//

	if (bWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	renderSceneToGBuffer(mView);

	if (bWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//
	// Render Lights to Accumulation Buffer
	//

#if LIGHT_PRE_PASS

	renderLightsToAccumBuffer();

#endif

	//
	// Render G-Buffer to Screen
	//

	if (FINAL != eRenderType)
	{
		renderIntermediateToScreen(eRenderType);
	}
	else
	{
		renderFinal();
	}
}

/**
 * @brief Rendering::onCreate
 * @param m
 */
void Rendering::onCreate(Mesh * m)
{
	m_aObjects.push_back(m->Instantiate());
}

/**
 * @brief Rendering::renderGBuffer
 */
void Rendering::renderSceneToGBuffer(const mat4x4 & mView)
{
	glViewport(0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight());

	GLuint uFBO = m_pGBuffer->GetObject();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uFBO);

#if LIGHT_PRE_PASS

	glDrawBuffer(GL_COLOR_ATTACHMENT2); // normal

#else

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; // position / diffuse / normal
	glDrawBuffers(3, DrawBuffers);

#endif

	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		mat4x4 mCameraViewProjection = m_pGBuffer->GetProjection() * mView;

		m_pGeometryPassShader->SetAsCurrent();

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mCameraViewProjection * object.transformation;

			m_pGeometryPassShader->SetUniform("ModelViewProjection", MVP);
			m_pGeometryPassShader->SetUniform("Model", object.transformation);

			object.mesh->draw();
		}

		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDrawBuffer(GL_BACK);
}

/**
 * @brief Rendering::renderLightsToAccumBuffer
 */
void Rendering::renderLightsToAccumBuffer()
{
	glViewport(0, 0, m_lightAccumBuffer.GetWidth(), m_lightAccumBuffer.GetHeight());

	// TODO : render lights
}

/**
 * @brief Rendering::renderSceneToShadowMap
 * @param mViewProjection
 */
void Rendering::renderSceneToShadowMap(void)
{
	glViewport(0, 0, m_pShadowMap->GetWidth(), m_pShadowMap->GetHeight());

	GLuint uFBO = m_pShadowMap->GetObject();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uFBO);

	glDrawBuffer(GL_NONE);

	glPolygonOffset(10.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	{
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		mat4x4 mDepthView = _lookAt(m_pLight->GetPosition(), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

		m_pDepthOnlyPassShader->SetAsCurrent();

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mDepthViewProjection * object.transformation;

			m_pDepthOnlyPassShader->SetUniform("ModelViewProjection", MVP);

			object.mesh->draw();
		}

		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
	}

	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDrawBuffer(GL_BACK);
}

/**
 * @brief Rendering::renderIntermediateToScreen
 * @param mViewProjection
 */
void Rendering::renderIntermediateToScreen(ERenderType eRenderType)
{
	glViewport(0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight());

	GLuint uFBO = m_pGBuffer->GetObject();

	switch (eRenderType)
	{
		case FINAL:
		{
			assert(0);
		}
		break;

		case POSITION:
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, uFBO);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBlitFramebuffer(0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), 0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		break;

		case DIFFUSE:
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, uFBO);
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glBlitFramebuffer(0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), 0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		break;

		case NORMAL:
		{
			m_pFullscreenNormalShader->SetAsCurrent();
			{
				m_pFullscreenDepthShader->SetTexture("texSampler", 0, m_pGBuffer->GetTexture(GBuffer::NORMAL));
				m_pQuadMesh->draw();
			}
			glUseProgram(0);
		}
		break;

		case DEPTH:
		{
			m_pFullscreenDepthShader->SetAsCurrent();
			{
				m_pFullscreenDepthShader->SetTexture("texSampler", 0, m_pGBuffer->GetTexture(GBuffer::DEPTH));
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
 * @brief Rendering::renderFinal
 * @param mViewProjection
 */
void Rendering::renderFinal(void)
{
#if LIGHT_PRE_PASS

#else

	glViewport(0, 0, m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight());

	glDepthMask(GL_FALSE);

	mat4x4 mDepthView = _lookAt(m_pLight->GetPosition(), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
	mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

	m_pFullscreenComposeShader->SetAsCurrent();
	{
		m_pFullscreenComposeShader->SetUniform("DepthTransformation", mDepthViewProjection);

		m_pFullscreenComposeShader->SetUniform("lightPos", m_pLight->GetPosition());

		m_pFullscreenComposeShader->SetTexture("diffuseSampler", 0, m_pGBuffer->GetTexture(GBuffer::DIFFUSE));
		m_pFullscreenComposeShader->SetTexture("normalSampler",	1, m_pGBuffer->GetTexture(GBuffer::NORMAL));
		m_pFullscreenComposeShader->SetTexture("positionSampler", 2, m_pGBuffer->GetTexture(GBuffer::POSITION));
		m_pFullscreenComposeShader->SetTexture("shadowMap",	3, m_pShadowMap->GetTexture());

		m_pQuadMesh->draw();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);

	glDepthMask(GL_TRUE);

#endif
}

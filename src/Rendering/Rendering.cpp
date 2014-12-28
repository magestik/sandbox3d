#include "Rendering.h"

#include <assert.h>

#include "utils.inl"

std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *> g_FragmentShaders;

std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *> g_VertexShaders;

std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

std::map<std::string, Mesh> g_Meshes;

#define SHADOW_MAP_SIZE 1024
#define DISABLE_BLIT 1

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
, m_pShadowMap(nullptr)
, m_GBuffer()
, m_LightAccumBuffer()
, m_Compose()
, m_pQuadMesh(nullptr)
{
	// ...
}

/**
 * @brief Rendering::onInitializeComplete
 */
void Rendering::onInitializeComplete()
{
    for (int i = 0; i < TARGET_MAX; ++i)
    {
        m_apTargets[i] = new GPU::Texture<GL_TEXTURE_2D>();
    }

	onResize(1280, 720);

    assert(m_GBuffer.init(m_apTargets[TARGET_NORMALS], m_apTargets[TARGET_DEPTH]));
    assert(m_Compose.init(m_apTargets[TARGET_FINAL_HDR], m_apTargets[TARGET_DEPTH]));
    assert(m_LightAccumBuffer.init(m_apTargets[TARGET_DIFFUSE_LIGHTS], m_apTargets[TARGET_SPECULAR_LIGHTS]));
    assert(m_AvLum.init(m_apTargets[TARGET_LUMINANCE1], m_apTargets[TARGET_LUMINANCE2]));
    assert(m_BloomPass.init(m_apTargets[TARGET_BLOOM1], m_apTargets[TARGET_BLOOM2]));

	{
		m_pLight = new Light::Directionnal(vec3(-20.0f, -20.0f, -20.0f));
		m_pShadowMap = new ShadowMap();
		m_pShadowMap->init(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
    }

    compileShaders();
    generateMeshes();

    glGenSamplers(1, &m_uSampler);
	glSamplerParameteri(m_uSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_uSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * @brief Compile all needed shaders
 */
void Rendering::compileShaders()
{
    m_pDirectionnalLightShader	= new Shader(g_VertexShaders["directionnal_light.vert"], g_FragmentShaders["directionnal_light.frag"]);

	m_pDepthOnlyPassShader		= new Shader(g_VertexShaders["depth_only.vert"], g_FragmentShaders["depth_only.frag"]);

	m_pFullscreenDepthShader	= new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_depth.frag"]);
	m_pFullscreenNormalShader	= new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_normal.frag"]);
    m_pFullscreenColorShader	= new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_color.frag"]);
    m_pFullscreenExpShader      = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["fullscreen_exp.frag"]);

    m_pToneMappingShader        = new Shader(g_VertexShaders["fullscreen.vert"], g_FragmentShaders["tonemapping.frag"]);
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
    m_uWidth        = width;
    m_uHeight       = height;

    m_uLuminanceSizePOT     = toPOT((width > height) ? width : height);

    float ratio = m_uWidth/(float)m_uHeight;
    m_matProjection = _perspective(75.0f, ratio, 1.0f, 1000.0f);

    // see https://www.opengl.org/wiki/Image_Format#Required_formats

    m_apTargets[TARGET_DEPTH  ]->init<GL_DEPTH_COMPONENT32F>(m_uWidth, m_uHeight);
    m_apTargets[TARGET_NORMALS]->init<GL_RGBA16F>(m_uWidth, m_uHeight);

    m_apTargets[TARGET_DIFFUSE_LIGHTS ]->init<GL_R11F_G11F_B10F>(m_uWidth, m_uHeight);
    m_apTargets[TARGET_SPECULAR_LIGHTS]->init<GL_R11F_G11F_B10F>(m_uWidth, m_uHeight);

    m_apTargets[TARGET_FINAL_HDR]->init<GL_R11F_G11F_B10F>(m_uWidth, m_uHeight);

    m_apTargets[TARGET_LUMINANCE1]->init<GL_R16F>(m_uLuminanceSizePOT, m_uLuminanceSizePOT);
    m_apTargets[TARGET_LUMINANCE2]->init<GL_R16F>(m_uLuminanceSizePOT, m_uLuminanceSizePOT);

    m_apTargets[TARGET_BLOOM1]->init<GL_R11F_G11F_B10F>(m_uWidth/4, m_uHeight/4);
    m_apTargets[TARGET_BLOOM2]->init<GL_R11F_G11F_B10F>(m_uWidth/4, m_uHeight/4);
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
    // Render Scene using light buffer
    //

    if (bWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    renderFinal(mView, clearColor, ambientColor);

    if (bWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //
    // Bloom
    //

    renderBloom();

    //
    // Tone Mapping
    //

    computeAverageLum();

    //
    // Draw to screen
    //

    renderIntermediateToScreen(eRenderType);

    //
    // Post process
    //

    if (eRenderType == FINAL)
    {
        renderPostProcessEffects();
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
 * @brief Rendering::computeAverageLum
 */
void Rendering::computeAverageLum(void)
{
    m_AvLum.begin();

    {
        glBindSampler(0, m_uSampler);

        glViewport(0, 0, m_uLuminanceSizePOT, m_uLuminanceSizePOT);

        m_AvLum.GetShader()->SetTexture("texSampler", 0, *(m_apTargets[TARGET_FINAL_HDR]));
        m_pQuadMesh->draw();

        vec2 texture_scale (1.0f, 1.0f);

        for (int size = m_uLuminanceSizePOT >> 1; size > 1; size >>= 1)
        {
            glViewport(0, 0, size, size);

            unsigned int tex = m_AvLum.next();

            m_AvLum.GetShader()->SetTexture("texSampler", 0, *(m_apTargets[TARGET_LUMINANCE1 + tex]));
            m_AvLum.GetShader()->SetUniform("textureScale", texture_scale);

            m_pQuadMesh->draw();

            texture_scale = texture_scale * 0.5f;
        }

        glBindSampler(0, 0);
    }

    m_AvLum.end();
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
            m_pToneMappingShader->SetAsCurrent();
            {
                float avLum = m_AvLum.getValue();
                m_pToneMappingShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_FINAL_HDR]));
                m_pToneMappingShader->SetUniform("avLum", avLum);
                m_pQuadMesh->draw();
            }
            glUseProgram(0);
		}
		break;

		case DIFFUSE_LIGHTS:
        {
#if DISABLE_BLIT
            m_pFullscreenColorShader->SetAsCurrent();
            {
                m_pFullscreenColorShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_DIFFUSE_LIGHTS]));
                m_pQuadMesh->draw();
            }
            glUseProgram(0);
#else
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_LightAccumBuffer.GetObject());
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#endif
		}
		break;

		case SPECULAR_LIGHTS:
        {
#if DISABLE_BLIT
            m_pFullscreenColorShader->SetAsCurrent();
            {
                m_pFullscreenColorShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_SPECULAR_LIGHTS]));
                m_pQuadMesh->draw();
            }
            glUseProgram(0);
#else
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_LightAccumBuffer.GetObject());
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#endif
		}
		break;

		case NORMAL_BUFFER:
		{
			m_pFullscreenNormalShader->SetAsCurrent();
			{
                m_pFullscreenNormalShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_NORMALS]));
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

        case LUMINANCE1:
        {
            m_pFullscreenExpShader->SetAsCurrent();
            {
                m_pFullscreenExpShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_LUMINANCE1]));
                m_pQuadMesh->draw();
            }
            glUseProgram(0);
        }
        break;

        case LUMINANCE2:
        {
            m_pFullscreenExpShader->SetAsCurrent();
            {
                m_pFullscreenExpShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_LUMINANCE2]));
                m_pQuadMesh->draw();
            }
            glUseProgram(0);
        }
        break;

        case BLOOM:
        {
            m_pFullscreenColorShader->SetAsCurrent();
            {
                m_pFullscreenColorShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_BLOOM1]));
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
                    m_GBuffer.enable_normalmapping();

                    m_GBuffer.GetShader()->SetTexture("normalMap", 0, *pNormalMap);
                }
                else
                {
                    m_GBuffer.disable_normalmapping();
                }

                m_GBuffer.GetShader()->SetUniform("shininess", m->m_material.shininess);
                m_GBuffer.GetShader()->SetUniform("ModelViewProjection", MVP);
                m_GBuffer.GetShader()->SetUniform("Model", object.transformation);

                m->draw();
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

    glBindSampler(3, m_uSampler);
    glBindSampler(4, m_uSampler);

	m_Compose.begin(clearColor);

	{
		mat4x4 mCameraViewProjection = m_matProjection * mView;

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

        m_Compose.GetShader()->SetTexture("diffuseLightSampler", 0, *(m_apTargets[TARGET_DIFFUSE_LIGHTS]));
        m_Compose.GetShader()->SetTexture("specularLightSampler", 1, *(m_apTargets[TARGET_SPECULAR_LIGHTS]));
        m_Compose.GetShader()->SetTexture("shadowMap", 2, m_pShadowMap->GetTexture());

        m_Compose.GetShader()->SetUniform("ambientColor", ambientColor.xyz);
        m_Compose.GetShader()->SetUniform("DepthTransformation", mDepthViewProjection);

		for (Mesh::Instance & object : m_aObjects)
		{
			mat4x4 MVP = mCameraViewProjection * object.transformation;

            m_Compose.GetShader()->SetUniform("ModelViewProjection", MVP);
            m_Compose.GetShader()->SetUniform("Model", object.transformation);

			for (SubMesh * m : object.getDrawCommands())
			{
                m_Compose.GetShader()->SetTexture("diffuseSampler", 3, *(m->m_material.m_diffuse));
                m_Compose.GetShader()->SetTexture("specularSampler", 4, *(m->m_material.m_specular));

				m->draw();
			}
        }
	}

    m_Compose.end();

    glBindSampler(3, 0);
    glBindSampler(4, 0);
}

/**
 * @brief Rendering::renderBloom
 */
void Rendering::renderBloom(void)
{
    glViewport(0, 0, m_uWidth/4, m_uHeight/4);

    m_BloomPass.begin();

    {
        //
        // bright pass
        m_BloomPass.GetShader()->SetTexture("texSampler", 0, *(m_apTargets[TARGET_FINAL_HDR]));

        m_pQuadMesh->draw();

        //
        // blur horizontal
        m_BloomPass.prepare_blur_h();

        m_BloomPass.GetShader()->SetTexture("texSampler", 0, *(m_apTargets[TARGET_BLOOM1]));

        m_pQuadMesh->draw();

        //
        // blur vertical
        m_BloomPass.prepare_blur_v();

        m_BloomPass.GetShader()->SetTexture("texSampler", 0, *(m_apTargets[TARGET_BLOOM2]));

        m_pQuadMesh->draw();
    }

    m_BloomPass.end();
}

/**
 * @brief Rendering::renderPostProcessEffects
 */
void Rendering::renderPostProcessEffects()
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glDepthMask(GL_FALSE);

    glBindSampler(0, m_uSampler);

    m_pFullscreenColorShader->SetAsCurrent();
    {
        m_pFullscreenColorShader->SetTexture("texSampler", 0, *(m_apTargets[TARGET_BLOOM1]));
        m_pQuadMesh->draw();
    }
    glUseProgram(0);

    glBindSampler(0, 0);

    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);
}

#include "Rendering.h"

#include <assert.h>

#include "utils.inl"

#include <tinyxml2.h>
#include "StrToGL.h"

#include "Pipeline.h"

using namespace tinyxml2;

std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *> g_FragmentShaders;

std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *> g_VertexShaders;

std::map<std::string, GPU::Shader<GL_GEOMETRY_SHADER> *> g_GeometryShaders;

std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

std::map<std::string, Mesh> g_Meshes;

#define SHADOW_MAP_SIZE 1024

static const char PIPELINE_STR []= "pipeline";

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
, m_pQuadMesh(nullptr)
, m_pCameraBuffer(nullptr)
, m_pObjectsBuffer(nullptr)
, m_mapTargets()
, m_mapTechnique()
{
    // ...
}

/**
 * @brief Rendering::onInitializeComplete
 */
void Rendering::onInitializeComplete()
{
    m_apTargets[TARGET_LUMINANCE1] = new GPU::Texture<GL_TEXTURE_2D>();
    m_apTargets[TARGET_LUMINANCE2] = new GPU::Texture<GL_TEXTURE_2D>();

    m_pCameraBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
    GPU::realloc(*m_pCameraBuffer, sizeof(CameraBlock), GL_STREAM_DRAW);

    m_pObjectsBuffer = new GPU::Buffer<GL_UNIFORM_BUFFER>();
    GPU::realloc(*m_pObjectsBuffer, sizeof(ObjectBlock)*m_aObjects.size(), GL_STREAM_DRAW);

    initializePipelineFromXML("data/render.xml");

    assert(m_AvLum.init(m_apTargets[TARGET_LUMINANCE1], m_apTargets[TARGET_LUMINANCE2]));

    {
        m_pLight = new Light::Directionnal(vec3(-20.0f, -20.0f, -20.0f));
        m_pShadowMap = new ShadowMap();
        m_pShadowMap->init(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
    }

    generateMeshes();

    glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_CAMERA, m_pCameraBuffer->GetObject(), 0, sizeof(CameraBlock));
}


/**
 * @brief Rendering::initializeFromXML
 * @param filename
 */
void Rendering::initializePipelineFromXML(const char * filename)
{
    XMLDocument doc;

    doc.LoadFile(filename);

    const XMLElement * root = doc.RootElement();

    const XMLElement * pipelines = root->FirstChildElement("pipelines");
    initializePipelines(pipelines);

    const XMLElement * targets = root->FirstChildElement("targets");
    initializeTargets(targets);

    onResize(m_uWidth, m_uHeight);

    const XMLElement * pipeline = root->FirstChildElement("techniques");
    initializeTechniques(pipeline);

    for (const std::pair<std::string, Technique> & p : m_mapTechnique)
    {
        p.second.SetUniformBlockBinding("CameraBlock", BLOCK_BINDING_CAMERA);
        p.second.SetUniformBlockBinding("ObjectBlock", BLOCK_BINDING_OBJECT);
    }
}

/**
 * @brief Rendering::initializePipelines
 * @param pipelines
 */
void Rendering::initializePipelines(const XMLElement * root)
{
    const XMLElement * elmt = root->FirstChildElement(PIPELINE_STR);

    while (nullptr != elmt)
    {
        const char * name = elmt->Attribute("name");

        assert(nullptr != name);

        m_mapPipeline[name] = new Pipeline(elmt, *this);

        elmt = elmt->NextSiblingElement(PIPELINE_STR);
    }
}

/**
 * @brief Rendering::initializeTargets
 * @param targets
 */
void Rendering::initializeTargets(const XMLElement * targets)
{
    const XMLElement * texture = targets->FirstChildElement("texture");

    while (NULL != texture)
    {
        const char * name = texture->Attribute("name");

        const char * format = texture->Attribute("format");

        unsigned int size_divisor = texture->UnsignedAttribute("size_divisor");

        if (0 == size_divisor)
        {
            size_divisor = 1;
        }

        RenderTexture RT(strToFormat(format), size_divisor);

        m_mapTargets.insert(std::pair<std::string, RenderTexture>(name, RT));

        texture = texture->NextSiblingElement("texture");
    }
}

/**
 * @brief Rendering::initializePipeline
 * @param pipeline
 */
void Rendering::initializeTechniques(const XMLElement * pipeline)
{
    const XMLElement * pass = pipeline->FirstChildElement("technique");

    while (NULL != pass)
    {
        const char * name = pass->Attribute("name");

        m_mapTechnique[name] = Technique(pass, *this);

        pass = pass->NextSiblingElement("technique");
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

    // see https://www.opengl.org/wiki/Image_Format#Required_formats

    m_apTargets[TARGET_LUMINANCE1]->init<GL_RG16F>(m_uLuminanceSizePOT, m_uLuminanceSizePOT);
    m_apTargets[TARGET_LUMINANCE2]->init<GL_RG16F>(m_uLuminanceSizePOT, m_uLuminanceSizePOT);

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
void Rendering::onUpdate(const mat4x4 & mView, const vec4 & clearColor, bool bWireframe, ERenderType eRenderType, const Mesh::Instance * pSelectedObject)
{
    updateCameraBuffer(mView);

    if (FINAL == eRenderType && bWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        renderSceneToGBuffer();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        renderLightsToAccumBuffer(mView);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        renderFinal(mView, clearColor);

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

    renderFinal(mView, clearColor);

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
    // Tone Mapping
    //

    computeAverageLum();

    //
    // Render to Screen
    //

    if (eRenderType == FINAL)
    {
        glViewport(0, 0, m_uWidth, m_uHeight);

        Technique & ToneMappingTechnique = m_mapTechnique["tonemapping"];

        ToneMappingTechnique.Begin();
        {
            float avLum = m_AvLum.getAverage();
            float white2 = m_AvLum.getMax2();
            ToneMappingTechnique.BeginPass("with_burnout");
            ToneMappingTechnique.SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));
            ToneMappingTechnique.SetUniform("avLum", avLum);
            ToneMappingTechnique.SetUniform("white2", white2);
            m_pQuadMesh->draw();
            ToneMappingTechnique.EndPass();
        }
        ToneMappingTechnique.End();

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

    Technique & PickBufferTechnique = m_mapTechnique["picking"];

    PickBufferTechnique.Begin();
    {
        PickBufferTechnique.BeginPass("default");

        unsigned int id = UINT32_MAX;

        PickBufferTechnique.ReadPixel(glPos, id);

        if (id < m_aObjects.size())
        {
            object = &(m_aObjects[id]);
        }

        PickBufferTechnique.EndPass();
    }
    PickBufferTechnique.End();

    return(object);
}

/**
 * @brief Rendering::computeAverageLum
 */
void Rendering::computeAverageLum(void)
{
    m_AvLum.begin();

    {
        glViewport(0, 0, m_uLuminanceSizePOT, m_uLuminanceSizePOT);

        m_AvLum.GetShader()->SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));
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

    m_pShadowMap->Begin();

    {
        mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
        mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

        m_pShadowMap->GetShader()->SetUniform("LightViewProjection", mDepthViewProjection);

        for (Mesh::Instance & object : m_aObjects)
        {
            m_pShadowMap->GetShader()->SetUniform("Model", object.transformation);

            object.mesh->bind();

            // TODO : remove loop and directly use glDrawElements on the full buffer
            for (SubMesh * m : object.getDrawCommands())
            {
                m->draw();
            }

            object.mesh->unbind();
        }

        glUseProgram(0);
    }

    m_pShadowMap->End();
}

/**
 * @brief Rendering::renderIntermediateToScreen
 * @param mViewProjection
 */
void Rendering::renderIntermediateToScreen(ERenderType eRenderType)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & DebugTechnique = m_mapTechnique["debug"];

    DebugTechnique.Begin();

    switch (eRenderType)
    {
        case FINAL:
        {
            DebugTechnique.BeginPass("color");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case DIFFUSE_LIGHTS:
        {
            DebugTechnique.BeginPass("color");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["lights_diffuse"].getTexture()));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case SPECULAR_LIGHTS:
        {
            DebugTechnique.BeginPass("color");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["lights_specular"].getTexture()));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case NORMAL_BUFFER:
        {
            DebugTechnique.BeginPass("normal");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["normals"].getTexture()));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case DEPTH:
        {
            DebugTechnique.BeginPass("depth");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_mapTargets["depth"].getTexture()));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case SHADOWS:
        {
            DebugTechnique.BeginPass("depth");
            {
                DebugTechnique.SetTexture("texSampler", 0, m_pShadowMap->GetTexture());
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case LUMINANCE1:
        {
            DebugTechnique.BeginPass("luminance");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_apTargets[TARGET_LUMINANCE1]));
                m_pQuadMesh->draw();
            }
            DebugTechnique.EndPass();
        }
        break;

        case LUMINANCE2:
        {
            DebugTechnique.BeginPass("luminance");
            {
                DebugTechnique.SetTexture("texSampler", 0, *(m_apTargets[TARGET_LUMINANCE2]));
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
    }

    DebugTechnique.End();
}

/**
 * @brief Rendering::renderGBuffer
 */
void Rendering::renderSceneToGBuffer(void)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & GeometryTechnique = m_mapTechnique["geometry"];

    GeometryTechnique.Begin();

    {
        // OPTIMIZE THIS !!!!!

        GeometryTechnique.BeginPass("simple");
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
                        GeometryTechnique.SetUniform("shininess", m->m_material.shininess);

                        m->draw();
                    }
                }

                object.mesh->unbind();

                ++offset;
            }
        }
        GeometryTechnique.EndPass();

        GeometryTechnique.BeginPass("normal_map");
        {
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
                        GeometryTechnique.SetTexture("normalMap", 0, *pNormalMap);
                        GeometryTechnique.SetUniform("shininess", m->m_material.shininess);

                        m->draw();
                    }
                }

                object.mesh->unbind();

                ++offset;
            }
        }
        GeometryTechnique.EndPass();
    }

    GeometryTechnique.End();
}

/**
 * @brief Rendering::renderLightsToAccumBuffer
 */
void Rendering::renderLightsToAccumBuffer(const mat4x4 & mView)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & LightsTechnique = m_mapTechnique["lights"];

    LightsTechnique.Begin();

    {
        LightsTechnique.BeginPass("directional");

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 mCameraViewProjection = m_matProjection * mView;

        {
            LightsTechnique.SetUniform("viewPos", (inverse(mView) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
            LightsTechnique.SetUniform("InverseViewProjection", inverse(mCameraViewProjection));
            LightsTechnique.SetUniform("lightDir", - normalize(m_pLight->GetDirection()));
            LightsTechnique.SetUniform("lightColor", m_pLight->GetColor());
            LightsTechnique.SetTexture("depthSampler", 0, *(m_mapTargets["depth"].getTexture()));
            LightsTechnique.SetTexture("normalSampler", 1, *(m_mapTargets["normals"].getTexture()));
            m_pQuadMesh->draw();
        }

        // TODO : render all lights

        LightsTechnique.EndPass();
    }

    LightsTechnique.End();
}

/**
 * @brief Rendering::renderFinal
 * @param mViewProjection
 */
void Rendering::renderFinal(const mat4x4 & mView, const vec4 & clearColor)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & ComposeTechnique = m_mapTechnique["compose"];

    ComposeTechnique.Begin();

    {
        ComposeTechnique.BeginPass("default");

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
        mat4x4 mDepthViewProjection = m_pShadowMap->GetProjection() * mDepthView;

        ComposeTechnique.SetTexture("diffuseLightSampler", 0, *(m_mapTargets["lights_diffuse"].getTexture()));
        ComposeTechnique.SetTexture("specularLightSampler", 1, *(m_mapTargets["lights_specular"].getTexture()));
        ComposeTechnique.SetTexture("shadowMap", 2, m_pShadowMap->GetTexture());

        ComposeTechnique.SetUniform("ambientColor", environment.ambient.Color);
        ComposeTechnique.SetUniform("DepthTransformation", mDepthViewProjection);
        ComposeTechnique.SetUniform("View", mView);

        unsigned int offset = 0;

        for (Mesh::Instance & object : m_aObjects)
        {
            glBindBufferRange(GL_UNIFORM_BUFFER, BLOCK_BINDING_OBJECT, m_pObjectsBuffer->GetObject(), sizeof(ObjectBlock)*offset, sizeof(ObjectBlock));

            object.mesh->bind();

            for (SubMesh * m : object.getDrawCommands())
            {
                ComposeTechnique.SetTexture("diffuseSampler", 3, *(m->m_material.m_diffuse));
                ComposeTechnique.SetTexture("specularSampler", 4, *(m->m_material.m_specular));

                m->draw();
            }

            object.mesh->unbind();

            ++offset;
        }

        ComposeTechnique.EndPass();
    }

    ComposeTechnique.End();
}

/**
 * @brief Rendering::renderFog
 * @param mView
 */
void Rendering::renderFog(void)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & ComposeTechnique = m_mapTechnique["fog"];

    ComposeTechnique.Begin();

    {
        ComposeTechnique.BeginPass("simple");

        ComposeTechnique.SetTexture("depthMapSampler", 0, *(m_mapTargets["depth"].getTexture()));
        ComposeTechnique.SetUniform("FogScattering", environment.fog.Scattering);
        ComposeTechnique.SetUniform("FogExtinction", environment.fog.Extinction);
        ComposeTechnique.SetUniform("FogColor", environment.fog.Color);

        float ratio = m_uWidth/(float)m_uHeight;

        ComposeTechnique.SetUniform("camera_near", 1.0f);
        ComposeTechnique.SetUniform("camera_far", 1000.0f);
        ComposeTechnique.SetUniform("near_plane_half_size", vec2(m_uHeight * (ratio), tanf(75.0f/2.0)));

        m_pQuadMesh->draw();

        ComposeTechnique.EndPass();
    }

    ComposeTechnique.End();
}

/**
 * @brief Rendering::renderBloom
 */
void Rendering::renderBloom(void)
{
    glViewport(0, 0, m_uWidth/4, m_uHeight/4);

    Technique & BloomTechnique = m_mapTechnique["bloom"];

    BloomTechnique.Begin();
    {
        BloomTechnique.BeginPass("bright");
        {
            BloomTechnique.SetTexture("texSampler", 0, *(m_mapTargets["HDR"].getTexture()));

            m_pQuadMesh->draw();
        }
        BloomTechnique.EndPass();

        BloomTechnique.BeginPass("horizontal_blur");
        {
            BloomTechnique.SetTexture("texSampler", 0, *(m_mapTargets["bloom1"].getTexture()));

            m_pQuadMesh->draw();
        }
        BloomTechnique.EndPass();

        BloomTechnique.BeginPass("vertical_blur");
        {
            BloomTechnique.SetTexture("texSampler", 0, *(m_mapTargets["bloom2"].getTexture()));

            m_pQuadMesh->draw();
        }
        BloomTechnique.EndPass();
    }
    BloomTechnique.End();
}

/**
 * @brief Rendering::renderPostProcessEffects
 */
void Rendering::renderPostProcessEffects()
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & BlendTechnique = m_mapTechnique["blend"];

    BlendTechnique.Begin();
    {
        BlendTechnique.BeginPass("bloom");

        BlendTechnique.SetTexture("texSampler", 0, *(m_mapTargets["bloom1"].getTexture()));

        m_pQuadMesh->draw();

        BlendTechnique.EndPass();
    }
    BlendTechnique.End();
}

/**
 * @brief Rendering::renderPickBuffer
 */
void Rendering::renderPickBuffer(void)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & PickBufferTechnique = m_mapTechnique["picking"];

    PickBufferTechnique.Begin();
    {
        PickBufferTechnique.BeginPass("default");

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int i = 0;

        for (Mesh::Instance & object : m_aObjects)
        {
            object.mesh->bind();

            PickBufferTechnique.SetUniform("Model", object.transformation);

            glVertexAttribI1ui(5, i);

            for (SubMesh * m : object.getDrawCommands())
            {
                m->draw();
            }

            object.mesh->unbind();

            ++i;
        }

        PickBufferTechnique.EndPass();
    }
    PickBufferTechnique.End();
}

/**
 * @brief Rendering::renderBoundingBox
 * @param mView
 */
void Rendering::renderBoundingBox(const Mesh::Instance * pSelectedObject)
{
    glViewport(0, 0, m_uWidth, m_uHeight);

    Technique & BBoxTechnique = m_mapTechnique["bbox"];

    BBoxTechnique.Begin();
    {
        BBoxTechnique.BeginPass("default");

        BBoxTechnique.SetUniform("Model", pSelectedObject->transformation);

        BBoxTechnique.SetUniform("BBoxMin", pSelectedObject->mesh->m_BoundingBox.min);
        BBoxTechnique.SetUniform("BBoxMax", pSelectedObject->mesh->m_BoundingBox.max);
        BBoxTechnique.SetUniform("color", vec3(1.0, 1.0, 1.0));

        m_pPointMesh->draw();

        BBoxTechnique.EndPass();
    }
    BBoxTechnique.End();
}

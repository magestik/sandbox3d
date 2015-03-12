#include "Technique.h"

#include "Rendering.h"
#include "Pass.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Technique::Technique
 */
Technique::Technique(void)
: m_pCurrentPass(nullptr)
, m_bActive(false)
{
    // ...
}

Technique::Technique(const XMLElement * root, const Rendering & rendering)
: m_pCurrentPass(nullptr)
, m_bActive(false)
{
    {
        const XMLElement * elmt = root->FirstChildElement("pass");

        while (nullptr != elmt)
        {
            const char * name = elmt->Attribute("name");

            assert(nullptr != name);

            const Pipeline * pipeline = nullptr;
            {
                const XMLElement * child = elmt->FirstChildElement("pipeline");

                assert(nullptr != child);

                const char * pipeline_name = child->Attribute("name");

                pipeline = rendering.GetPipeline(pipeline_name);

                assert(nullptr != pipeline);
            }

            m_mapPass[name] = Pass(pipeline, elmt, rendering);

            elmt = elmt->NextSiblingElement("pass");
        }
    }
}

/**
 * @brief Technique::begin
 * @return
 */
bool Technique::Begin(void)
{
    assert(!m_bActive);
    assert(nullptr == m_pCurrentPass);

    m_bActive = true;

    return(true);
}

/**
 * @brief Technique::end
 */
void Technique::End(void)
{
    assert(m_bActive);
    assert(nullptr == m_pCurrentPass);

    m_bActive = false;
}

/**
 * @brief Technique::operator ()
 * @param pass
 */
bool Technique::BeginPass(const char * pass)
{
    assert(m_bActive);
    assert(nullptr == m_pCurrentPass);

    m_pCurrentPass = &(m_mapPass[pass]);

    m_pCurrentPass->Begin();

    return(true);
}

/**
 * @brief Technique::EndPass
 */
void Technique::EndPass(void)
{
    assert(m_bActive);
    assert(nullptr != m_pCurrentPass);

    m_pCurrentPass->End();

    m_pCurrentPass = nullptr;
}

/**
 * @brief Technique::ReadPixel
 * @param pos
 * @param result
 * @return
 */
bool Technique::ReadPixel(const ivec2 & pos, unsigned int & result)
{
    assert(m_bActive);
    assert(nullptr != m_pCurrentPass);

    return(m_pCurrentPass->ReadPixel(pos, result));
}

/**
 * @brief Technique::SetUniformBlockBinding
 * @param name
 * @param binding
 */
void Technique::SetUniformBlockBinding(const char *name, unsigned int binding) const
{
    for (const std::pair<std::string, Pass> & p : m_mapPass)
    {
        p.second.SetUniformBlockBinding(name, binding);
    }
}

/**
 * @brief Technique::SetUniform
 * @param name
 * @param value
 */
template<typename T>
void Technique::SetUniform(const char * name, const T & value)
{
    assert(m_bActive);
    m_pCurrentPass->SetUniform(name, value);
}

template void Technique::SetUniform(const char * name, const mat2x2 & value);
template void Technique::SetUniform(const char * name, const mat3x3 & value);
template void Technique::SetUniform(const char * name, const mat4x4 & value);

template void Technique::SetUniform(const char * name, const vec2 & value);
template void Technique::SetUniform(const char * name, const vec3 & value);
template void Technique::SetUniform(const char * name, const vec4 & value);

template void Technique::SetUniform(const char * name, const int & value);
template void Technique::SetUniform(const char * name, const unsigned int & value);
template void Technique::SetUniform(const char * name, const float & value);

/**
 * @brief Technique::SetTexture
 * @param name
 * @param binding
 * @param texture
 */
template<GLenum D>
void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<D> & texture)
{
    assert(m_bActive);
    m_pCurrentPass->SetTexture(name, unit, texture);
}

template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

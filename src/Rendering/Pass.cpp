#include "Pass.h"

#include "Rendering.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Pass::Pass
 */
Pass::Pass(void)
: m_uFramebufferObject(0)
, m_uShaderObject(0)
{
    // ...
}

/**
 * @brief Pass::init
 * @param element
 * @param rendering
 * @return
 */
Pass::Pass(const XMLElement * element, const Rendering & rendering)
: m_uFramebufferObject(0)
, m_uShaderObject(0)
{
    //
    // Framebuffer
    {
        const XMLElement * output = element->FirstChildElement("output");

        if (NULL != output)
        {
            glGenFramebuffers(1, &m_uFramebufferObject);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

            GLenum current_color_attachment = GL_COLOR_ATTACHMENT0;

            while (NULL != output)
            {
                const char * texture	= output->Attribute("texture");
                const char * buffer		= output->Attribute("buffer");

                assert(!(nullptr != texture && nullptr != buffer));

                if (nullptr != texture)
                {
                    GLenum a = current_color_attachment;

                    const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = rendering.GetRenderTexture(texture);

                    const GLint format = pRenderTexture->getFormat();

                    if (GPU::isDepthStencilFormat(format))
                    {
                        a = GL_DEPTH_STENCIL_ATTACHMENT;
                    }
                    else if (GPU::isDepthFormat(format))
                    {
                        a = GL_DEPTH_ATTACHMENT;
                    }
                    else if (GPU::isStencilFormat(format))
                    {
                        a = GL_STENCIL_ATTACHMENT;
                    }
                    else
                    {
                        current_color_attachment += 1;
                        m_aDrawBuffers.push_back(a);
                    }

                    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, a, GL_TEXTURE_2D, pRenderTexture->GetObject(), 0);
                }
                else if (nullptr != buffer)
                {

                }

                output = output->NextSiblingElement("output");
            }

            GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            assert(GL_FRAMEBUFFER_COMPLETE == status);
        }
    }

    //
    // Shaders
    {
        m_uShaderObject = glCreateProgram();

        const XMLElement * shader = element->FirstChildElement("shader");

        while (NULL != shader)
        {
            const char * name = shader->Attribute("name");

            assert(nullptr != name);

            const char * type = shader->Attribute("type");

            switch (strToShaderType(type))
            {
                case GL_VERTEX_SHADER:
                {
                    glAttachShader(m_uShaderObject, rendering.GetVertexShader(name)->GetObject());
                }
                break;

                case GL_FRAGMENT_SHADER:
                {
                    glAttachShader(m_uShaderObject, rendering.GetFragmentShader(name)->GetObject());
                }
                break;

                case GL_GEOMETRY_SHADER:
                {
                    glAttachShader(m_uShaderObject, rendering.GetGeometryShader(name)->GetObject());
                }
                break;
            }

            shader = shader->NextSiblingElement("shader");
        }

        glLinkProgram(m_uShaderObject);

        int status = 0;
        glGetProgramiv(m_uShaderObject, GL_LINK_STATUS, &status);

        assert(GL_TRUE == status);
    }

    //
    // Samplers
    {
        const XMLElement * sampler = element->FirstChildElement("sampler");

        while (NULL != sampler)
        {
            GLuint samplerObject = 0;
            glGenSamplers(1, &samplerObject);

            const char * name = sampler->Attribute("name");

            assert(nullptr != name);

            const char * min_filter = sampler->Attribute("min_filter");

            if (nullptr != min_filter)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_MIN_FILTER, strToMinFilter(min_filter));
            }

            const char * mag_filter = sampler->Attribute("mag_filter");

            if (nullptr != mag_filter)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_MAG_FILTER, strToMagFilter(mag_filter));
            }

            const char * wrap_s = sampler->Attribute("wrap_s");

            if (nullptr != wrap_s)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_S, strToWrapMode(wrap_s));
            }

            const char * wrap_t = sampler->Attribute("wrap_t");

            if (nullptr != wrap_t)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_T, strToWrapMode(wrap_t));
            }

            const char * wrap_r = sampler->Attribute("wrap_r");

            if (nullptr != wrap_r)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_R, strToWrapMode(wrap_r));
            }

            const char * compare_mode = sampler->Attribute("compare_mode");

            if (nullptr != compare_mode)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_COMPARE_MODE, strToCompareMode(compare_mode));
            }

            const char * compare_func = sampler->Attribute("compare_func");

            if (nullptr != compare_func)
            {
                glSamplerParameteri(samplerObject, GL_TEXTURE_COMPARE_FUNC, strToCompareFunc(compare_func));
            }

            m_mapSamplers.insert(std::pair<std::string, GLuint>(name, samplerObject));

            sampler = sampler->NextSiblingElement("sampler");
        }
    }
}

/**
 * @brief Pass::~Pass
 */
Pass::~Pass(void)
{
#if 0
    glDeleteFramebuffers(1, &m_uFramebufferObject);

    GLuint shaders [10];
    GLsizei size = 0;
    glGetAttachedShaders(m_uShaderObject, 10, &size, shaders);

    for (int i = 0; i < size; ++i)
    {
        glDetachShader(m_uShaderObject, shaders[i]);
    }

    glDeleteProgram(m_uShaderObject);
#endif
}

/**
 * @brief Pass::begin
 */
bool Pass::Begin(void)
{
    unsigned int size = m_aDrawBuffers.size();

    if (size > 0)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);
        glDrawBuffers(m_aDrawBuffers.size(), m_aDrawBuffers.data());
    }

    glUseProgram(m_uShaderObject);

    return(true);
}

/**
 * @brief Pass::end
 */
void Pass::End(void)
{
    glUseProgram(0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

/**
 * @brief Pass::ReadPixel
 * @param pos
 * @param result
 * @return
 */
bool Pass::ReadPixel(const ivec2 & pos, unsigned int & result)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uFramebufferObject);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glReadPixels(pos.x, pos.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &result);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_BACK);

    return(true);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, const mat2x2 & m)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniformMatrix2fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, const mat3x3 & m)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniformMatrix3fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, const mat4x4 & m)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniformMatrix4fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param v
 */
void Pass::SetUniform(const char * name, const vec4 & v)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform4fv(location, 1, (float*)&v);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param v
 */
void Pass::SetUniform(const char * name, const vec3 & v)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform3fv(location, 1, (float*)&v);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param v
 */
void Pass::SetUniform(const char * name, const vec2 & v)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform2fv(location, 1, (float*)&v);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, int n)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform1i(location, n);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, unsigned int n)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform1ui(location, n);
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param m
 */
void Pass::SetUniform(const char * name, float n)
{
    GLint location = glGetUniformLocation(m_uShaderObject, name);
    //assert(-1 != location);
    glUniform1f(location, n);
}

/**
 * @brief Pass::SetConstantBufferBinding
 * @param name
 * @param binding
 */
void Pass::SetUniformBlockBinding(const char * name, unsigned int binding) const
{
    GLuint blockIndex = glGetUniformBlockIndex(m_uShaderObject, name);

    if (GL_INVALID_INDEX != blockIndex)
    {
        glUniformBlockBinding(m_uShaderObject, blockIndex, binding);
    }
}

/**
 * @brief Pass::SetTexture
 * @param name
 * @param unit
 * @param texture
 */
template<GLenum D>
void Pass::SetTexture(const char * name, int unit, const GPU::Texture<D> & texture)
{
    SetUniform(name, unit);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(D, texture.GetObject());
    glBindSampler(unit, m_mapSamplers[name]);
}

template void Pass::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Pass::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Pass::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

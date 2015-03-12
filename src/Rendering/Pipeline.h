#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>
#include <string>
#include <map>

namespace tinyxml2
{
    class XMLElement;
}

class Rendering;

struct DepthControl
{
    DepthControl(void)
    {
        enable	= false;
        mask	= GL_TRUE;
        func	= GL_LESS;
    }

    bool enable;

    GLboolean mask;
    GLenum func;
};

struct BlendControl
{
    BlendControl(void)
    {
        enable		= false;
        sfactor		= GL_ONE;
        sfactor		= GL_ZERO;
        equation	= GL_FUNC_ADD;
    }

    bool enable;

    GLenum sfactor;
    GLenum dfactor;
    GLenum equation;
};

class Pipeline
{
public:

    Pipeline();
    Pipeline(const tinyxml2::XMLElement * element, const Rendering & rendering);

    bool        Bind            (void) const;
    void        UnBind          (void) const;

    void		SetUniform		(const char * name, const mat4x4 & m) const;
    void		SetUniform		(const char * name, const mat3x3 & m) const;
    void		SetUniform		(const char * name, const mat2x2 & m) const ;

    void		SetUniform		(const char * name, const vec4 & v) const;
    void		SetUniform		(const char * name, const vec3 & v) const;
    void		SetUniform		(const char * name, const vec2 & v) const;

    void		SetUniform		(const char * name, int n) const;
    void		SetUniform		(const char * name, unsigned int n) const;
    void		SetUniform		(const char * name, float n) const;

    void        SetUniformBlockBinding  (const char * name, unsigned int binding) const;

    template<GLenum D>
    void		SetTexture		(const char * name, int unit, const GPU::Texture<D> & texture) const;

protected:

    std::vector<GLenum> m_aEnable;

    DepthControl m_sDepthControl;
    BlendControl m_sBlendControl;

    GLuint m_uShaderObject;

    std::map<std::string, GLuint> m_mapSamplers;

};

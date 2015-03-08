#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>
#include <map>

namespace tinyxml2
{
    class XMLElement;
}

class Rendering;
class Shader;

class Pass
{

public:

    explicit	Pass			(void);
    explicit	Pass			(const tinyxml2::XMLElement * element, const Rendering & rendering);
    virtual		~Pass			(void);

    bool		Begin			(void);
    void		End				(void);

    bool		ReadPixel		(const ivec2 & pos, unsigned int & result);

    void		SetUniform		(const char * name, const mat4x4 & m);
    void		SetUniform		(const char * name, const mat3x3 & m);
    void		SetUniform		(const char * name, const mat2x2 & m);

    void		SetUniform		(const char * name, const vec4 & v);
    void		SetUniform		(const char * name, const vec3 & v);
    void		SetUniform		(const char * name, const vec2 & v);

    void		SetUniform		(const char * name, int n);
    void		SetUniform		(const char * name, unsigned int n);
    void		SetUniform		(const char * name, float n);

    void        SetUniformBlockBinding  (const char * name, unsigned int binding) const;

    template<GLenum D>
    void		SetTexture		(const char * name, int unit, const GPU::Texture<D> & texture);

protected:

    GLuint m_uFramebufferObject;
    GLuint m_uShaderObject;
    std::map<std::string, GLuint> m_mapSamplers;

    std::vector<GLenum> m_aDrawBuffers;
};

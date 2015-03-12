#pragma once

#include <GPU.h>

#include <Vector.h>

#include <vector>
#include <map>

#include "Pipeline.h"

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
    explicit	Pass			(const Pipeline * pipeline);
    explicit	Pass			(const Pipeline * pipeline, const tinyxml2::XMLElement * element, const Rendering & rendering);
    virtual		~Pass			(void);

    bool		Begin			(void);
    void		End				(void);

    bool		ReadPixel		(const ivec2 & pos, unsigned int & result);

    void SetUniformBlockBinding  (const char * name, unsigned int binding) const
    {
        m_Pipeline->SetUniformBlockBinding(name, binding);
    }

    template<typename T>
    void SetUniform (const char * name, const T & value) const
    {
        m_Pipeline->SetUniform(name, value);
    }

    template<GLenum D>
    void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture) const
    {
        m_Pipeline->SetTexture(name, binding, texture);
    }

protected:

    GLuint m_uFramebufferObject;

    const Pipeline * m_Pipeline;

    std::vector<GLenum> m_aDrawBuffers;
};

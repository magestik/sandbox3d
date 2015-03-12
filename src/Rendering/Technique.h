#pragma once

#include <map>
#include <vector>
#include <string>

#include <GPU.h>

#include <Vector.h>

#include "Pipeline.h"

namespace tinyxml2
{
    class XMLElement;
}

class Rendering;
class Pass;

class Technique
{
public:

    Technique();
    Technique(const tinyxml2::XMLElement * element, const Rendering & rendering);

    bool	Begin		(void);
    void	End			(void);

    bool	BeginPass	(const char * pass);
    void	EndPass		(void);

    bool	ReadPixel	(const ivec2 & pos, unsigned int & result);

    void    SetUniformBlockBinding  (const char * name, unsigned int binding) const;

    template<typename T>
    void SetUniform (const char * name, const T & value);

    template<GLenum D>
    void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture);

protected:

    std::map<std::string, Pass> m_mapPass;

    Pass * m_pCurrentPass;

    bool m_bActive;
};

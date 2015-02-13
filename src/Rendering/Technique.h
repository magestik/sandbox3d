#pragma once

#include <map>
#include <vector>
#include <string>

#include <GPU.h>

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

	template<typename T>
	void SetUniform (const char * name, const T & value);

	template<GLenum D>
	void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture);

protected:

	std::map<std::string, Pass> m_mapPass;

	std::vector<GLenum> m_aEnable;

	GLenum m_eDepthFunc;

	Pass * m_pCurrentPass;

};

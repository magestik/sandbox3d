#pragma once

#include "Pass.h"

#include <map>
#include <string>

namespace tinyxml2
{
	class XMLElement;
}

class Rendering;

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
	void SetUniform (const char * name, const T & value)
	{
		m_pCurrentPass->SetUniform(name, value);
	}

	template<GLenum D>
	void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture)
	{
		m_pCurrentPass->SetTexture(name, binding, texture);
	}

protected:

	std::map<std::string, Pass> m_mapPass;

	Pass * m_pCurrentPass;

};

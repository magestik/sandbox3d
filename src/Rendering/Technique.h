#pragma once

#include <map>
#include <vector>
#include <string>

#include <GPU.h>

#include <Vector.h>

namespace tinyxml2
{
	class XMLElement;
}

class Rendering;
class Pass;

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

	template<typename T>
	void SetUniform (const char * name, const T & value);

	template<GLenum D>
	void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture);

protected:

	std::map<std::string, Pass> m_mapPass;

	std::vector<GLenum> m_aEnable;

	Pass * m_pCurrentPass;

	DepthControl m_sDepthControl;
	BlendControl m_sBlendControl;

	bool m_bActive;
};

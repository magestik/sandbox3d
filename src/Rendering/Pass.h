#pragma once

#include <map>
#include <vector>
#include <string>

#include <GPU.h>

#include <Vector.h>

#include "Pipeline.h"
#include "Subpass.h"

namespace tinyxml2
{
	class XMLElement;
}

class Rendering;

class Pass
{
public:

				Pass			(void);
				Pass			(const Pass & pass) = delete;
				Pass			(Pass && pass);
	explicit	Pass			(const tinyxml2::XMLElement * element, const Rendering & rendering);
				~Pass			(void);

	Pass &		operator =		(Pass && pass);

	bool	BeginRenderPass		(void);
	void	EndRenderPass		(void);

	bool	BeginPass			(const char * pass);
	void	EndPass				(void);

	bool	ReadPixel (const ivec2 & pos, unsigned int & result);

	void    SetUniformBlockBinding  (const char * name, unsigned int binding) const;

	template<typename T>
	void SetUniform (const char * name, const T & value);

	template<GLenum D>
	void SetTexture (const char * name, unsigned int binding, const GPU::Texture<D> & texture);

protected:

	std::map<std::string, Subpass> m_mapPass;

	Subpass * m_pCurrentPass;

	int m_iSubpassCount;

	bool m_bActive; // just for debugging
};

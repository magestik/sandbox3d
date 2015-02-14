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

Technique::Technique(const XMLElement * element, const Rendering & rendering)
: m_pCurrentPass(nullptr)
, m_bActive(false)
{
	{
		const XMLElement * pass = element->FirstChildElement("pass");

		while (nullptr != pass)
		{
			const char * name = pass->Attribute("name");

			assert(nullptr != name);

			m_mapPass[name] = Pass(pass, rendering);

			pass = pass->NextSiblingElement("pass");
		}
	}

	{
		const XMLElement * enable = element->FirstChildElement("enable");

		while (nullptr != enable)
		{
			const char * cap = enable->Attribute("cap");

			assert(nullptr != cap);

			m_aEnable.push_back(strToCapability(cap));

			enable = enable->NextSiblingElement("enable");
		}
	}

	{
		const XMLElement * depth = element->FirstChildElement("depth");

		if (nullptr != depth)
		{
			m_sDepthControl.enable = true;

			const char * func = depth->Attribute("func");

			if (nullptr != func)
			{
				m_sDepthControl.func = strToDepthFunc(func);
			}

			const char * mask = depth->Attribute("mask");

			if (nullptr != mask)
			{
				m_sDepthControl.mask = strToDepthMask(mask);
			}
		}
	}

	{
		const XMLElement * blend = element->FirstChildElement("blend");

		if (nullptr != blend)
		{
			m_sBlendControl.enable = true;

			const char * equation = blend->Attribute("equation");

			if (nullptr != equation)
			{
				m_sBlendControl.equation = strToBlendEquation(equation);
			}

			const char * sfactor = blend->Attribute("sfactor");

			if (nullptr != sfactor)
			{
				m_sBlendControl.sfactor = strToBlendFunc(sfactor);
			}

			const char * dfactor = blend->Attribute("dfactor");

			if (nullptr != dfactor)
			{
				m_sBlendControl.dfactor = strToBlendFunc(dfactor);
			}
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

	for (GLenum cap : m_aEnable)
	{
		glEnable(cap);
	}

	if (m_sDepthControl.enable)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(m_sDepthControl.func);
		glDepthMask(m_sDepthControl.mask);
	}

	if (m_sBlendControl.enable)
	{
		glEnable(GL_BLEND);
		glBlendEquation(m_sBlendControl.equation);
		glBlendFunc(m_sBlendControl.sfactor, m_sBlendControl.dfactor);
	}

	return(true);
}

/**
 * @brief Technique::end
 */
void Technique::End(void)
{
	assert(m_bActive);
	assert(nullptr == m_pCurrentPass);

	if (m_sBlendControl.enable)
	{
		glDisable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
	}

	if (m_sDepthControl.enable)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}

	for (GLenum cap : m_aEnable)
	{
		glDisable(cap);
	}

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

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
, m_eDepthFunc(GL_LESS)
{
	// ...
}

Technique::Technique(const XMLElement * element, const Rendering & rendering)
: m_pCurrentPass(nullptr)
{
	{
		const XMLElement * pass = element->FirstChildElement("pass");

		while (NULL != pass)
		{
			const char * name = pass->Attribute("name");

			m_mapPass[name] = Pass(pass, rendering);

			pass = pass->NextSiblingElement("pass");
		}
	}

	{
		const XMLElement * enable = element->FirstChildElement("enable");

		while (NULL != enable)
		{
			const char * cap = enable->Attribute("cap");

			m_aEnable.push_back(strToCapability(cap));

			enable = enable->NextSiblingElement("enable");
		}
	}
}

/**
 * @brief Technique::begin
 * @return
 */
bool Technique::Begin(void)
{
	for (GLenum cap : m_aEnable)
	{
		glEnable(cap);
	}

	return(true);
}

/**
 * @brief Technique::end
 */
void Technique::End(void)
{
	for (GLenum cap : m_aEnable)
	{
		glDisable(cap);
	}
}

/**
 * @brief Technique::operator ()
 * @param pass
 */
bool Technique::BeginPass(const char * pass)
{
	m_pCurrentPass = &(m_mapPass[pass]);

	m_pCurrentPass->Begin();

	return(true);
}

/**
 * @brief Technique::EndPass
 */
void Technique::EndPass(void)
{
	m_pCurrentPass->End();

	m_pCurrentPass = nullptr;
}

/**
 * @brief Technique::SetUniform
 * @param name
 * @param value
 */
template<typename T>
void Technique::SetUniform(const char * name, const T & value)
{
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
	m_pCurrentPass->SetTexture(name, unit, texture);
}

template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Technique::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

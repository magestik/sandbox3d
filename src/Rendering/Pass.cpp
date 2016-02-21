#include "Pass.h"

#include "Rendering.h"
#include "Subpass.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Pass::Pass
 */
Pass::Pass(void)
: m_iCurrentSubpass(0)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Move constructor
 * @param pass
 */
Pass::Pass(Pass && pass)
: m_iCurrentSubpass(pass.m_iCurrentSubpass)
, m_bActive(pass.m_bActive)
{
	pass.m_iCurrentSubpass = 0;
	pass.m_bActive = false;
}

/**
 * @brief Pass::Pass
 * @param root
 * @param rendering
 */
Pass::Pass(const XMLElement * root, const Rendering & rendering)
: m_iCurrentSubpass(0)
, m_bActive(false)
{
	{
		const XMLElement * elmt = root->FirstChildElement("subpass");

		while (nullptr != elmt)
		{
			const char * name = elmt->Attribute("name");

			assert(nullptr != name);

			const char * pipeline_name = elmt->Attribute("pipeline");

			assert(nullptr != pipeline_name);

			const Pipeline * pipeline = rendering.GetPipeline(pipeline_name);

			m_aPass.push_back(Subpass(pipeline, elmt, rendering));

			elmt = elmt->NextSiblingElement("subpass");
		}
	}
}

/**
 * @brief Pass::~Pass
 */
Pass::~Pass(void)
{

}

/**
 * @brief Pass::operator =
 * @param pass
 * @return
 */
Pass & Pass::operator=(Pass && pass)
{
	m_iCurrentSubpass = pass.m_iCurrentSubpass;
	pass.m_iCurrentSubpass = 0;

	m_bActive = pass.m_bActive;
	pass.m_bActive = false;

	m_aPass = std::move(pass.m_aPass);

	return(*this);
}

/**
 * @brief Pass::Begin
 * @return
 */
bool Pass::BeginRenderPass(void)
{
	assert(!m_bActive);
	assert(m_iCurrentSubpass == 0);

	m_bActive = true;

	m_aPass.front().Begin();

	return(true);
}

/**
 * @brief Pass::EndRenderPass
 */
void Pass::EndRenderPass(void)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass == (m_aPass.size() - 1));

	m_aPass.back().End();

	m_bActive = false;
	m_iCurrentSubpass = 0;
}

/**
 * @brief Pass::NextSubpass
 */
void Pass::NextSubpass(void)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass < m_aPass.size());

	m_aPass[m_iCurrentSubpass].End();

	++m_iCurrentSubpass;

	m_aPass[m_iCurrentSubpass].Begin();
}

/**
 * @brief Pass::ReadPixel
 * @param pos
 * @param result
 * @return
 */
bool Pass::ReadPixel(const ivec2 & pos, unsigned int & result)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass >= 0 && m_iCurrentSubpass < m_aPass.size());

	return(m_aPass[m_iCurrentSubpass].ReadPixel(pos, result));
}

/**
 * @brief Pass::SetUniformBlockBinding
 * @param name
 * @param binding
 */
void Pass::SetUniformBlockBinding(const char *name, unsigned int binding) const
{
	for (const Subpass & p : m_aPass)
	{
		p.SetUniformBlockBinding(name, binding);
	}
}

/**
 * @brief Pass::SetUniform
 * @param name
 * @param value
 */
template<typename T>
void Pass::SetUniform(const char * name, const T & value)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass >= 0 && m_iCurrentSubpass < m_aPass.size());

	m_aPass[m_iCurrentSubpass].SetUniform(name, value);
}

template void Pass::SetUniform(const char * name, const mat2x2 & value);
template void Pass::SetUniform(const char * name, const mat3x3 & value);
template void Pass::SetUniform(const char * name, const mat4x4 & value);

template void Pass::SetUniform(const char * name, const vec2 & value);
template void Pass::SetUniform(const char * name, const vec3 & value);
template void Pass::SetUniform(const char * name, const vec4 & value);

template void Pass::SetUniform(const char * name, const int & value);
template void Pass::SetUniform(const char * name, const unsigned int & value);
template void Pass::SetUniform(const char * name, const float & value);

/**
 * @brief Pass::SetTexture
 * @param name
 * @param unit
 * @param texture
 */
template<GLenum D>
void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<D> & texture)
{
	assert(m_bActive);
	assert(m_iCurrentSubpass >= 0 && m_iCurrentSubpass < m_aPass.size());

	m_aPass[m_iCurrentSubpass].SetTexture(name, unit, texture);
}

template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

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
: m_pCurrentPass(nullptr)
, m_bActive(false)
{
	// ...
}

/**
 * @brief Pass::Pass
 * @param root
 * @param rendering
 */
Pass::Pass(const XMLElement * root, const Rendering & rendering)
: m_pCurrentPass(nullptr)
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

			m_mapPass[name] = Subpass(pipeline, elmt, rendering);

			elmt = elmt->NextSiblingElement("subpass");
		}
	}
}

/**
 * @brief Pass::Begin
 * @return
 */
bool Pass::BeginRenderPass(void)
{
	assert(!m_bActive);
	assert(nullptr == m_pCurrentPass);

	m_bActive = true;

	return(true);
}

/**
 * @brief Pass::EndRenderPass
 */
void Pass::EndRenderPass(void)
{
	assert(m_bActive);
	assert(nullptr == m_pCurrentPass);

	m_bActive = false;
}

/**
 * @brief Pass::BeginPass
 * @param pass
 * @return
 */
bool Pass::BeginPass(const char * pass)
{
	assert(m_bActive);
	assert(nullptr == m_pCurrentPass);

	m_pCurrentPass = &(m_mapPass[pass]);

	m_pCurrentPass->Begin();

	return(true);
}

/**
 * @brief Pass::EndPass
 */
void Pass::EndPass(void)
{
	assert(m_bActive);
	assert(nullptr != m_pCurrentPass);

	m_pCurrentPass->End();

	m_pCurrentPass = nullptr;
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
	assert(nullptr != m_pCurrentPass);

	return(m_pCurrentPass->ReadPixel(pos, result));
}

/**
 * @brief Pass::SetUniformBlockBinding
 * @param name
 * @param binding
 */
void Pass::SetUniformBlockBinding(const char *name, unsigned int binding) const
{
	for (const std::pair<std::string, Subpass> & p : m_mapPass)
	{
		p.second.SetUniformBlockBinding(name, binding);
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
	m_pCurrentPass->SetUniform(name, value);
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
	m_pCurrentPass->SetTexture(name, unit, texture);
}

template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_1D> & texture);
template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_2D> & texture);
template void Pass::SetTexture(const char * name, unsigned int unit, const GPU::Texture<GL_TEXTURE_3D> & texture);

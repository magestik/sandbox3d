#include "Technique.h"

#include "../Rendering.h"

#include <tinyxml2.h>

using namespace tinyxml2;

static inline GLenum strToCap(const char * strCap);

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

			m_aEnable.push_back(strToCap(cap));

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

	m_pCurrentPass->begin();

	return(true);
}

/**
 * @brief Technique::EndPass
 */
void Technique::EndPass(void)
{
	m_pCurrentPass->end();

	m_pCurrentPass = nullptr;
}

/**
 * @brief strToCap
 * @param strCap
 * @return
 */
static inline GLenum strToCap(const char * strCap)
{
	if (!strcmp(strCap, "GL_DEPTH_TEST"))
	{
		return(GL_DEPTH_TEST);
	}
	else
	{
		assert(false);
	}

	return(0);
}

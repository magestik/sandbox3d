#include "Technique.h"

#include "../Rendering.h"

#include <tinyxml2.h>

using namespace tinyxml2;

/**
 * @brief Technique::Technique
 */
Technique::Technique(void)
: m_pCurrentPass(nullptr)
{
	// ...
}

Technique::Technique(const XMLElement * element, const Rendering & rendering)
: m_pCurrentPass(nullptr)
{
	const XMLElement * pass = element->FirstChildElement("pass");

	while (NULL != pass)
	{
		const char * name = pass->Attribute("name");

		m_mapPass[name] = Pass(pass, rendering);

		pass = pass->NextSiblingElement("pass");
	}
}

/**
 * @brief Technique::begin
 * @return
 */
bool Technique::Begin(void)
{
	return(true);
}

/**
 * @brief Technique::end
 */
void Technique::End(void)
{
	// ...
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

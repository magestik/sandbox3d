#include "Pipeline.h"

#include "Rendering.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Pipeline::Pipeline
 */
Pipeline::Pipeline()
{
	// ...
}

/**
 * @brief Pipeline::Pipeline
 * @param element
 * @param rendering
 */
Pipeline::Pipeline(const XMLElement * root, const Rendering & rendering)
{
	// Samplers
	{
		const XMLElement * elmt = root->FirstChildElement("sampler");

		while (NULL != elmt)
		{
			GLuint samplerObject = 0;

			glGenSamplers(1, &samplerObject);

			const char * name = elmt->Attribute("name");

			assert(nullptr != name);

			const char * min_filter = elmt->Attribute("min_filter");

			if (nullptr != min_filter)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_MIN_FILTER, strToMinFilter(min_filter));
			}

			const char * mag_filter = elmt->Attribute("mag_filter");

			if (nullptr != mag_filter)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_MAG_FILTER, strToMagFilter(mag_filter));
			}

			const char * wrap_s = elmt->Attribute("wrap_s");

			if (nullptr != wrap_s)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_S, strToWrapMode(wrap_s));
			}

			const char * wrap_t = elmt->Attribute("wrap_t");

			if (nullptr != wrap_t)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_T, strToWrapMode(wrap_t));
			}

			const char * wrap_r = elmt->Attribute("wrap_r");

			if (nullptr != wrap_r)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_WRAP_R, strToWrapMode(wrap_r));
			}

			const char * compare_mode = elmt->Attribute("compare_mode");

			if (nullptr != compare_mode)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_COMPARE_MODE, strToCompareMode(compare_mode));
			}

			const char * compare_func = elmt->Attribute("compare_func");

			if (nullptr != compare_func)
			{
				glSamplerParameteri(samplerObject, GL_TEXTURE_COMPARE_FUNC, strToCompareFunc(compare_func));
			}

			m_mapSamplers.insert(std::pair<std::string, GLuint>(name, samplerObject));

			elmt = elmt->NextSiblingElement("sampler");
		}
	}
}

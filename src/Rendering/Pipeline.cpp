#include "Pipeline.h"

#include "Rendering.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Pipeline::Pipeline
 */
Pipeline::Pipeline()
: m_uShaderObject(0)
{
	// ...
}

/**
 * @brief Pipeline::Pipeline
 * @param element
 * @param rendering
 */
Pipeline::Pipeline(const XMLElement * root, const Rendering & rendering)
: m_uShaderObject(0)
{
	RHI::Pipeline::InputAssemblyState inputAssemblyState;
	RHI::Pipeline::BlendState blendState;
	RHI::Pipeline::DepthStencilState depthStencilState;

	// misc glEnable(...)
	{
		const XMLElement * enable = root->FirstChildElement("enable");

		while (nullptr != enable)
		{
			const char * cap = enable->Attribute("cap");

			assert(nullptr != cap);

			m_aEnable.push_back(strToCapability(cap));

			enable = enable->NextSiblingElement("enable");
		}
	}

	// Depth Control
	{
		const XMLElement * depth = root->FirstChildElement("depth");

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

	// Stencil Control
	{
		const XMLElement * stencil = root->FirstChildElement("stencil");

		if (nullptr != stencil)
		{
			m_sDepthControl.enable = true;

			const char * func = stencil->Attribute("func");

			if (nullptr != func)
			{
				m_sDepthControl.func = strToStencilFunc(func);
			}

			const char * mask = stencil->Attribute("mask");

			if (nullptr != mask)
			{
				m_sDepthControl.mask = strToStencilMask(mask);
			}
		}
	}

	// Blend Control
	{
		const XMLElement * blend = root->FirstChildElement("blend");

		if (nullptr != blend)
		{
			blendState.enable = true;

			const char * equation = blend->Attribute("equation");

			if (nullptr != equation)
			{
				blendState.colorEquation = blendState.alphaEquation = RHI::BlendOp(strToBlendEquation(equation));
			}

			const char * sfactor = blend->Attribute("sfactor");

			if (nullptr != sfactor)
			{
				blendState.srcColorFactor = blendState.srcAlphaFactor = RHI::BlendFactor(strToBlendFunc(sfactor));
			}

			const char * dfactor = blend->Attribute("dfactor");

			if (nullptr != dfactor)
			{
				blendState.dstColorFactor = blendState.dstAlphaFactor = RHI::BlendFactor(strToBlendFunc(dfactor));
			}
		}
	}

	// Shaders
	{
		m_uShaderObject = glCreateProgram();

		const XMLElement * shader = root->FirstChildElement("shader");

		while (NULL != shader)
		{
			const char * name = shader->Attribute("name");

			assert(nullptr != name);

			const char * type = shader->Attribute("type");

			switch (strToShaderType(type))
			{
				case GL_VERTEX_SHADER:
				{
					glAttachShader(m_uShaderObject, rendering.GetVertexShader(name)->GetObject());
				}
				break;

				case GL_FRAGMENT_SHADER:
				{
					glAttachShader(m_uShaderObject, rendering.GetFragmentShader(name)->GetObject());
				}
				break;

				case GL_GEOMETRY_SHADER:
				{
					glAttachShader(m_uShaderObject, rendering.GetGeometryShader(name)->GetObject());
				}
				break;
			}

			shader = shader->NextSiblingElement("shader");
		}

		glLinkProgram(m_uShaderObject);

		int status = 0;
		glGetProgramiv(m_uShaderObject, GL_LINK_STATUS, &status);

		assert(GL_TRUE == status);
	}

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

	m_pipeline = RHI::Pipeline(inputAssemblyState, depthStencilState, blendState);
}

/**
 * @brief Pipeline::OnBind
 * @return
 */
bool Pipeline::Bind(void) const
{
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

	if (m_sStencilControl.enable)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(m_sStencilControl.func, 0, UINT32_MAX);
		glStencilMask(m_sStencilControl.mask);
	}
#if 0
	if (m_sBlendControl.enable)
	{
		glEnable(GL_BLEND);
		glBlendEquation(m_sBlendControl.equation);
		glBlendFunc(m_sBlendControl.sfactor, m_sBlendControl.dfactor);
	}
#endif
	glUseProgram(m_uShaderObject);

	return(true);
}

/**
 * @brief Pipeline::OnUnBind
 */
void Pipeline::UnBind(void) const
{
	glUseProgram(0);

	if (m_sStencilControl.enable)
	{
		glDisable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, UINT32_MAX);
		glStencilMask(UINT32_MAX);
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
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, const mat2x2 & m) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix2fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, const mat3x3 & m) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix3fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, const mat4x4 & m) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix4fv(location, 1, false, (float*)&m);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param v
 */
void Pipeline::SetUniform(const char * name, const vec4 & v) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform4fv(location, 1, (float*)&v);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param v
 */
void Pipeline::SetUniform(const char * name, const vec3 & v) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform3fv(location, 1, (float*)&v);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param v
 */
void Pipeline::SetUniform(const char * name, const vec2 & v) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform2fv(location, 1, (float*)&v);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, int n) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform1i(location, n);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, unsigned int n) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform1ui(location, n);
}

/**
 * @brief Pipeline::SetUniform
 * @param name
 * @param m
 */
void Pipeline::SetUniform(const char * name, float n) const
{
	GLint location = glGetUniformLocation(m_uShaderObject, name);
	//assert(-1 != location);
	glUniform1f(location, n);
}

/**
 * @brief Pipeline::SetConstantBufferBinding
 * @param name
 * @param binding
 */
void Pipeline::SetUniformBlockBinding(const char * name, unsigned int binding) const
{
	GLuint blockIndex = glGetUniformBlockIndex(m_uShaderObject, name);

	if (GL_INVALID_INDEX != blockIndex)
	{
		glUniformBlockBinding(m_uShaderObject, blockIndex, binding);
	}
}

/**
 * @brief Pipeline::SetTexture
 * @param name
 * @param unit
 * @param texture
 */
template<GLenum D>
void Pipeline::SetTexture(const char * name, int unit, const GPU::Texture<D> & texture) const
{
	SetUniform(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(D, texture.GetObject());
	glBindSampler(unit, m_mapSamplers.at(name));
}

template void Pipeline::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_1D> & texture) const;
template void Pipeline::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_2D> & texture) const;
template void Pipeline::SetTexture(const char * name, int unit, const GPU::Texture<GL_TEXTURE_3D> & texture) const;

#include "StrToGL.h"

#include <assert.h>

// TODO : Use loop everywhere !!!

/**
 * @brief strToAttachment
 * @param strAttachment
 * @return
 */
GLenum strToAttachment(const char * strAttachment)
{
	if (!strcmp(strAttachment, "GL_DEPTH_ATTACHMENT"))
	{
		return(GL_DEPTH_ATTACHMENT);
	}
	else if (!strcmp(strAttachment, "GL_STENCIL_ATTACHMENT"))
	{
		return(GL_STENCIL_ATTACHMENT);
	}
	else if (!strcmp(strAttachment, "GL_DEPTH_STENCIL_ATTACHMENT"))
	{
		return(GL_DEPTH_STENCIL_ATTACHMENT);
	}
	else if (!strcmp(strAttachment, "GL_COLOR_ATTACHMENT0"))
	{
		return(GL_COLOR_ATTACHMENT0);
	}
	else if (!strcmp(strAttachment, "GL_COLOR_ATTACHMENT1"))
	{
		return(GL_COLOR_ATTACHMENT1);
	}
	else if (!strcmp(strAttachment, "GL_COLOR_ATTACHMENT2"))
	{
		return(GL_COLOR_ATTACHMENT2);
	}
	else if (!strcmp(strAttachment, "GL_COLOR_ATTACHMENT3"))
	{
		return(GL_COLOR_ATTACHMENT3);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToShaderType
 * @param strShaderType
 * @return
 */
GLenum strToShaderType(const char * strShaderType)
{
	if (!strcmp(strShaderType, "GL_VERTEX_SHADER"))
	{
		return(GL_VERTEX_SHADER);
	}
	else if (!strcmp(strShaderType, "GL_FRAGMENT_SHADER"))
	{
		return(GL_FRAGMENT_SHADER);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToCapability
 * @param strCapability
 * @return
 */
GLenum strToCapability(const char * strCapability)
{
	if (!strcmp(strCapability, "GL_DEPTH_TEST"))
	{
		return(GL_DEPTH_TEST);
	}
	else if (!strcmp(strCapability, "GL_BLEND"))
	{
		return(GL_BLEND);
	}
	else if (!strcmp(strCapability, "GL_POLYGON_OFFSET_FILL"))
	{
		return(GL_POLYGON_OFFSET_FILL);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToFormat
 * @param strFormat
 * @return
 */
GLenum strToFormat(const char * strFormat)
{
	if (!strcmp(strFormat, "GL_RGBA16F"))
	{
		return(GL_RGBA16F);
	}
	else if (!strcmp(strFormat, "GL_RGBA32F"))
	{
		return(GL_RGBA32F);
	}
	else if (!strcmp(strFormat, "GL_R32UI"))
	{
		return(GL_R32UI);
	}
	else if (!strcmp(strFormat, "GL_RGB10_A2​"))
	{
		return(GL_RGB10_A2);
	}
	else if (!strcmp(strFormat, "GL_RGB10_A2UI​"))
	{
		return(GL_RGB10_A2UI);
	}
	else if (!strcmp(strFormat, "GL_R11F_G11F_B10F"))
	{
		return(GL_R11F_G11F_B10F);
	}
	else if (!strcmp(strFormat, "GL_DEPTH_COMPONENT16"))
	{
		return(GL_DEPTH_COMPONENT16);
	}
	else if (!strcmp(strFormat, "GL_DEPTH_COMPONENT24"))
	{
		return(GL_DEPTH_COMPONENT24);
	}
	else if (!strcmp(strFormat, "GL_DEPTH_COMPONENT32F"))
	{
		return(GL_DEPTH_COMPONENT32F);
	}
	else if (!strcmp(strFormat, "GL_DEPTH24_STENCIL8"))
	{
		return(GL_DEPTH24_STENCIL8);
	}
	else if (!strcmp(strFormat, "GL_DEPTH32F_STENCIL8"))
	{
		return(GL_DEPTH32F_STENCIL8);
	}
	else if (!strcmp(strFormat, "GL_STENCIL_INDEX8"))
	{
		return(GL_STENCIL_INDEX8);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToDepthMask
 * @param strDepthMask
 * @return
 */
GLenum strToDepthMask(const char * strDepthMask)
{
	if (!strcmp(strDepthMask, "GL_TRUE"))
	{
		return(GL_TRUE);
	}
	else if (!strcmp(strDepthMask, "GL_FALSE"))
	{
		return(GL_FALSE);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToDepthFunc
 * @param strBoolean
 * @return
 */
GLenum strToDepthFunc(const char * strDepthFunc)
{
	if (!strcmp(strDepthFunc, "GL_EQUAL"))
	{
		return(GL_EQUAL);
	}
	else if (!strcmp(strDepthFunc, "GL_NOTEQUAL"))
	{
		return(GL_NOTEQUAL);
	}
	else if (!strcmp(strDepthFunc, "GL_GEQUAL"))
	{
		return(GL_GEQUAL);
	}
	else if (!strcmp(strDepthFunc, "GL_LEQUAL"))
	{
		return(GL_LEQUAL);
	}
	else if (!strcmp(strDepthFunc, "GL_GREATER"))
	{
		return(GL_GREATER);
	}
	else if (!strcmp(strDepthFunc, "GL_LESS"))
	{
		return(GL_LESS);
	}
	else if (!strcmp(strDepthFunc, "GL_NEVER"))
	{
		return(GL_NEVER);
	}
	else if (!strcmp(strDepthFunc, "GL_AWAYS"))
	{
		return(GL_ALWAYS);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToBlendEquation
 * @param strBlendEquation
 * @return
 */
GLenum strToBlendEquation(const char * strBlendEquation)
{
	if (!strcmp(strBlendEquation, "GL_FUNC_ADD"))
	{
		return(GL_FUNC_ADD);
	}
	else if (!strcmp(strBlendEquation, "GL_FUNC_SUBTRACT"))
	{
		return(GL_FUNC_SUBTRACT);
	}
	else if (!strcmp(strBlendEquation, "GL_FUNC_REVERSE_SUBTRACT"))
	{
		return(GL_FUNC_REVERSE_SUBTRACT);
	}
	else if (!strcmp(strBlendEquation, "GL_MIN"))
	{
		return(GL_MIN);
	}
	else if (!strcmp(strBlendEquation, "GL_MAX"))
	{
		return(GL_MAX);
	}
	else
	{
		assert(false);
	}

	return(0);
}

/**
 * @brief strToBlendFunc
 * @param strBlendFunc
 * @return
 */
GLenum strToBlendFunc(const char * strBlendFunc)
{
	if (!strcmp(strBlendFunc, "GL_ONE"))
	{
		return(GL_ONE);
	}
	else if (!strcmp(strBlendFunc, "GL_ZERO"))
	{
		return(GL_ZERO);
	}
	else
	{
		assert(false);
	}

	return(0);
}

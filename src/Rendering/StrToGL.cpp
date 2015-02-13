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

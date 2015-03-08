#include "StrToGL.h"

#include <assert.h>

// TODO : Use loop everywhere !!!

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
    else if (!strcmp(strShaderType, "GL_GEOMETRY_SHADER"))
    {
        return(GL_GEOMETRY_SHADER);
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
    else if (!strcmp(strBlendFunc, "GL_SRC_COLOR"))
    {
        return(GL_SRC_COLOR);
    }
    else if (!strcmp(strBlendFunc, "GL_DST_COLOR"))
    {
        return(GL_DST_COLOR);
    }
    else if (!strcmp(strBlendFunc, "GL_SRC_ALPHA"))
    {
        return(GL_SRC_ALPHA);
    }
    else if (!strcmp(strBlendFunc, "GL_DST_ALPHA"))
    {
        return(GL_DST_ALPHA);
    }
    else
    {
        assert(false);
    }

    return(0);
}

/**
 * @brief strToMinFilter
 * @param strFilter
 * @return
 */
GLenum strToMinFilter(const char * strFilter)
{
    if (!strcmp(strFilter, "GL_NEAREST"))
    {
        return(GL_NEAREST);
    }
    else if (!strcmp(strFilter, "GL_LINEAR"))
    {
        return(GL_LINEAR);
    }
    else if (!strcmp(strFilter, "GL_NEAREST_MIPMAP_NEAREST"))
    {
        return(GL_NEAREST_MIPMAP_NEAREST);
    }
    else if (!strcmp(strFilter, "GL_LINEAR_MIPMAP_NEAREST"))
    {
        return(GL_LINEAR_MIPMAP_NEAREST);
    }
    else if (!strcmp(strFilter, "GL_NEAREST_MIPMAP_LINEAR"))
    {
        return(GL_NEAREST_MIPMAP_LINEAR);
    }
    else if (!strcmp(strFilter, "GL_LINEAR_MIPMAP_LINEAR"))
    {
        return(GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        assert(false);
    }

    return(0);
}

/**
 * @brief strToMagFilter
 * @param strFilter
 * @return
 */
GLenum strToMagFilter(const char * strFilter)
{
    if (!strcmp(strFilter, "GL_NEAREST"))
    {
        return(GL_NEAREST);
    }
    else if (!strcmp(strFilter, "GL_LINEAR"))
    {
        return(GL_LINEAR);
    }
    else
    {
        assert(false);
    }

    return(0);
}

/**
 * @brief strToWrapMode
 * @param strWrapMode
 * @return
 */
GLenum strToWrapMode(const char * strWrapMode)
{
    if (!strcmp(strWrapMode, "GL_CLAMP_TO_EDGE"))
    {
        return(GL_CLAMP_TO_EDGE);
    }
    else if (!strcmp(strWrapMode, "GL_MIRRORED_REPEAT"))
    {
        return(GL_MIRRORED_REPEAT);
    }
    else if (!strcmp(strWrapMode, "GL_REPEAT"))
    {
        return(GL_REPEAT);
    }
    else if (!strcmp(strWrapMode, "GL_CLAMP_TO_BORDER"))
    {
        return(GL_CLAMP_TO_BORDER);
    }
    else
    {
        assert(false);
    }

    return(0);
}

/**
 * @brief strToCompareMode
 * @param strCompareMode
 * @return
 */
GLenum strToCompareMode(const char * strCompareMode)
{
    if (!strcmp(strCompareMode, "GL_COMPARE_REF_TO_TEXTURE"))
    {
        return(GL_COMPARE_REF_TO_TEXTURE);
    }
    else if (!strcmp(strCompareMode, "GL_NONE"))
    {
        return(GL_NONE);
    }
    else
    {
        assert(false);
    }

    return(0);
}

/**
 * @brief strToCompareFunc
 * @param strCompareFunc
 * @return
 */
GLenum strToCompareFunc(const char * strCompareFunc)
{
    if (!strcmp(strCompareFunc, "GL_EQUAL"))
    {
        return(GL_EQUAL);
    }
    else if (!strcmp(strCompareFunc, "GL_NOTEQUAL"))
    {
        return(GL_NOTEQUAL);
    }
    else if (!strcmp(strCompareFunc, "GL_GEQUAL"))
    {
        return(GL_GEQUAL);
    }
    else if (!strcmp(strCompareFunc, "GL_LEQUAL"))
    {
        return(GL_LEQUAL);
    }
    else if (!strcmp(strCompareFunc, "GL_GREATER"))
    {
        return(GL_GREATER);
    }
    else if (!strcmp(strCompareFunc, "GL_LESS"))
    {
        return(GL_LESS);
    }
    else if (!strcmp(strCompareFunc, "GL_NEVER"))
    {
        return(GL_NEVER);
    }
    else if (!strcmp(strCompareFunc, "GL_AWAYS"))
    {
        return(GL_ALWAYS);
    }
    else
    {
        assert(false);
    }

    return(0);
}

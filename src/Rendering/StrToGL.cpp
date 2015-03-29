#include "StrToGL.h"

#include <assert.h>

#include <stdlib.h>

#define STR2ENUM(str, aString, aValues) \
for (unsigned int i = 0; i < sizeof(aValues)/sizeof(aValues[0]); ++i) \
{ \
	if (!strcmp(str, aStrings[i])) \
	{ \
		return(aValues[i]); \
	} \
}

#define DECLARE_STRINGS(aStrings) static const char * aStrings [] = ENUM_LIST
#define DECLARE_VALUES(aValues) static GLenum aValues [] = ENUM_LIST

/**
 * @brief strToShaderType
 * @param strShaderType
 * @return
 */
GLenum strToShaderType(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_VERTEX_SHADER), \
		X(GL_FRAGMENT_SHADER), \
		X(GL_GEOMETRY_SHADER) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToCapability
 * @param strCapability
 * @return
 */
GLenum strToCapability(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_DEPTH_TEST), \
		X(GL_BLEND), \
		X(GL_POLYGON_OFFSET_FILL) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToFormat
 * @param strFormat
 * @return
 */
GLenum strToFormat(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_R8), \
		X(GL_RG8), \
		X(GL_RGBA8), \
		X(GL_R16F), \
		X(GL_RG16F), \
		X(GL_RGBA16F), \
		X(GL_R32F), \
		X(GL_RG32F), \
		X(GL_RGBA32F), \
		X(GL_R32UI), \
		X(GL_RG32UI), \
		X(GL_RGBA32UI), \
		X(GL_RGB10_A2), \
		X(GL_RGB10_A2UI), \
		X(GL_R11F_G11F_B10F), \
		X(GL_DEPTH_COMPONENT16), \
		X(GL_DEPTH_COMPONENT24), \
		X(GL_DEPTH_COMPONENT32F), \
		X(GL_DEPTH24_STENCIL8), \
		X(GL_DEPTH32F_STENCIL8), \
		X(GL_STENCIL_INDEX8) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToDepthMask
 * @param strDepthMask
 * @return
 */
GLenum strToDepthMask(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_TRUE), \
		X(GL_FALSE) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToDepthFunc
 * @param strBoolean
 * @return
 */
GLenum strToDepthFunc(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_EQUAL), \
		X(GL_NOTEQUAL), \
		X(GL_GEQUAL), \
		X(GL_LEQUAL), \
		X(GL_GREATER), \
		X(GL_LESS), \
		X(GL_NEVER), \
		X(GL_ALWAYS) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToBlendEquation
 * @param strBlendEquation
 * @return
 */
GLenum strToBlendEquation(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_FUNC_ADD), \
		X(GL_FUNC_SUBTRACT), \
		X(GL_FUNC_REVERSE_SUBTRACT), \
		X(GL_MIN), \
		X(GL_MAX) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToBlendFunc
 * @param strBlendFunc
 * @return
 */
GLenum strToBlendFunc(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_ONE), \
		X(GL_ZERO), \
		X(GL_SRC_COLOR), \
		X(GL_DST_COLOR), \
		X(GL_SRC_ALPHA), \
		X(GL_DST_ALPHA) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToMinFilter
 * @param strFilter
 * @return
 */
GLenum strToMinFilter(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_NEAREST), \
		X(GL_LINEAR), \
		X(GL_NEAREST_MIPMAP_NEAREST), \
		X(GL_LINEAR_MIPMAP_NEAREST), \
		X(GL_NEAREST_MIPMAP_LINEAR), \
		X(GL_LINEAR_MIPMAP_LINEAR) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToMagFilter
 * @param strFilter
 * @return
 */
GLenum strToMagFilter(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_NEAREST), \
		X(GL_LINEAR) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToWrapMode
 * @param strWrapMode
 * @return
 */
GLenum strToWrapMode(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_CLAMP_TO_EDGE), \
		X(GL_MIRRORED_REPEAT), \
		X(GL_REPEAT), \
		X(GL_CLAMP_TO_BORDER) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToCompareMode
 * @param strCompareMode
 * @return
 */
GLenum strToCompareMode(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_COMPARE_REF_TO_TEXTURE), \
		X(GL_NONE) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToCompareFunc
 * @param strCompareFunc
 * @return
 */
GLenum strToCompareFunc(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_EQUAL), \
		X(GL_NOTEQUAL), \
		X(GL_LEQUAL), \
		X(GL_GREATER), \
		X(GL_LESS), \
		X(GL_NEVER), \
		X(GL_ALWAYS) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToStencilFunc
 * @param strDepthFunc
 * @return
 */
GLenum strToStencilFunc(const char * str)
{
	#define ENUM_LIST \
	{ \
		X(GL_EQUAL), \
		X(GL_NOTEQUAL), \
		X(GL_LEQUAL), \
		X(GL_GREATER), \
		X(GL_LESS), \
		X(GL_NEVER), \
		X(GL_ALWAYS) \
	}

	#define X(e) #e
	DECLARE_STRINGS(aStrings);
	#undef X

	#define X(e) e
	DECLARE_VALUES(aValues);
	#undef X

	STR2ENUM(str, aStrings, aValues)

	#undef ENUM_LIST

	assert(false);
	return(0);
}

/**
 * @brief strToStencilMask
 * @param strDepthMask
 * @return
 */
GLuint strToStencilMask(const char * strStencilMask)
{
	return(strtoul(strStencilMask, nullptr, 2));
}

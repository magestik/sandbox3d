#pragma once

#include "config.h"

#include <GPU.h>

GLenum strToShaderType		(const char * strShaderType);
GLenum strToCapability		(const char * strCapability);
GLenum strToFormat			(const char * strFormat);

GLenum strToDepthFunc		(const char * strDepthFunc);
GLenum strToDepthMask		(const char * strDepthMask);

GLenum strToStencilFunc		(const char * strStencilFunc);
GLuint strToStencilMask		(const char * strStencilMask);

GLenum strToBlendEquation	(const char * strBlendEquation);
GLenum strToBlendFunc		(const char * strBlendFunc);

GLenum strToMinFilter		(const char * strFilter);
GLenum strToMagFilter		(const char * strFilter);
GLenum strToWrapMode		(const char * strWrapMode);
GLenum strToCompareMode     (const char * strCompareMode);
GLenum strToCompareFunc     (const char * strCompareFunc);

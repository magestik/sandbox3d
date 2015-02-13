#pragma once

#include <GPU.h>

GLenum strToAttachment		(const char * strAttachment);
GLenum strToShaderType		(const char * strShaderType);
GLenum strToCapability		(const char * strCapability);
GLenum strToFormat			(const char * strFormat);

GLenum strToDepthFunc		(const char * strDepthFunc);
GLenum strToDepthMask		(const char * strDepthMask);

GLenum strToBlendEquation	(const char * strBlendEquation);
GLenum strToBlendFunc		(const char * strBlendFunc);

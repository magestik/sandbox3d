#pragma once

unsigned int /*GLenum*/ strToShaderType		(const char * strShaderType);
unsigned int /*GLenum*/ strToCapability		(const char * strCapability);
unsigned int /*GLenum*/ strToFormat			(const char * strFormat);

unsigned int /*GLenum*/ strToDepthFunc		(const char * strDepthFunc);
unsigned int /*GLenum*/ strToDepthMask		(const char * strDepthMask);

unsigned int /*GLenum*/ strToStencilFunc	(const char * strStencilFunc);
unsigned int /*GLuint*/ strToStencilMask	(const char * strStencilMask);

unsigned int /*GLenum*/ strToBlendEquation	(const char * strBlendEquation);
unsigned int /*GLenum*/ strToBlendFunc		(const char * strBlendFunc);

unsigned int /*GLenum*/ strToMinFilter		(const char * strFilter);
unsigned int /*GLenum*/ strToMagFilter		(const char * strFilter);
unsigned int /*GLenum*/ strToWrapMode		(const char * strWrapMode);
unsigned int /*GLenum*/ strToCompareMode	(const char * strCompareMode);
unsigned int /*GLenum*/ strToCompareFunc	(const char * strCompareFunc);

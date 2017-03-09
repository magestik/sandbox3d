#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief Default constructor
 */
RHI::ShaderModule::ShaderModule(void)
: m_uShaderObject(0)
{
	// ...
}

/**
 * @brief Constructor
 */
RHI::ShaderModule::ShaderModule(ShaderModuleCreateInfo & createInfo)
: m_uShaderObject(0)
{
	GLint length = createInfo.codeSize;
	m_uShaderObject = glCreateShader(createInfo.stage);
	glShaderSource(m_uShaderObject, 1, (GLchar**)&createInfo.pCode, &length);
	glCompileShader(m_uShaderObject);
	GLint isCompiled = 0;
	glGetShaderiv(m_uShaderObject, GL_COMPILE_STATUS, &isCompiled);
	assert(GL_TRUE == isCompiled);
}

/**
 * @brief Destructor
 */
RHI::ShaderModule::~ShaderModule(void)
{
	// ...
}

#endif // HAVE_OPENGL

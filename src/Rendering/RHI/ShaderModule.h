#pragma once

namespace RHI
{

struct ShaderModuleCreateInfo
{
	ShaderStage stage;
	unsigned int codeSize;
	void * pCode;
};

class ShaderModule
{
public:

	//
	// Constructors / Destructor
	ShaderModule		(void);
	ShaderModule		(ShaderModuleCreateInfo & createInfo);
	~ShaderModule		(void);

public:

#if HAVE_OPENGL
	GLuint m_uShaderObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkShaderModule m_shaderModule;
#endif // HAVE_VULKAN
};

}

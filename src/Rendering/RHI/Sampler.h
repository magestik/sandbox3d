#pragma once

namespace RHI
{

class Sampler
{
public:

	friend class CommandBuffer;

	//
	// Constructors / Destructor
	Sampler			(void);
	~Sampler		(void);

protected:

#if HAVE_OPENGL
	GLuint m_uSamplerObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkSampler m_sampler;
#endif // HAVE_VULKAN
};

}

#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief Default constructor
 */
RHI::Sampler::Sampler(void)
: m_uSamplerObject(0)
{
	// ...
}

/**
 * @brief Constructor
 */
RHI::Sampler::Sampler(const SamplerCreateInfo & createInfo)
: m_uSamplerObject(0)
{
	glGenSamplers(1, &m_uSamplerObject);

	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_MIN_FILTER, createInfo.minFilter);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_MAG_FILTER, createInfo.magFilter);

	// TODO : mipmapMode

	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_S, createInfo.addressModeU);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_T, createInfo.addressModeV);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_R, createInfo.addressModeW);

	// TODO : mipLodBias
	// TODO : anisotropyEnable
	// TODO : maxAnisotropy

	if (createInfo.compareEnable)
	{
		glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_COMPARE_FUNC, createInfo.compareOp);
	}
	else
	{
		glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}

	// TODO : minLod
	// TODO : maxLod
	// TODO : borderColor
	// TODO : unnormalizedCoordinates
}

/**
 * @brief Destructor
 */
RHI::Sampler::~Sampler(void)
{
	// ...
}

#endif // HAVE_OPENGL

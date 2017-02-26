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
RHI::Sampler::Sampler(const SamplerCreateInfo & sample)
: m_uSamplerObject(0)
{
	glGenSamplers(1, &m_uSamplerObject);


	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_MIN_FILTER, sample.minFilter);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_MAG_FILTER, sample.magFilter);

	// TODO : mipmapMode

	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_S, sample.addressModeU);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_T, sample.addressModeV);
	glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_WRAP_R, sample.addressModeW);

	// TODO : mipLodBias
	// TODO : anisotropyEnable
	// TODO : maxAnisotropy

	if (sample.compareEnable)
	{
		glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_uSamplerObject, GL_TEXTURE_COMPARE_FUNC, sample.compareOp);
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

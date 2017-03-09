#pragma once

namespace RHI
{

struct SamplerCreateInfo
{
	SamplerCreateInfo(void)
	{
		magFilter = FILTER_NEAREST;
		minFilter = FILTER_NEAREST;
		//mipmapMode = ;
		addressModeU = SAMPLER_ADDRESS_MODE_REPEAT;
		addressModeV = SAMPLER_ADDRESS_MODE_REPEAT;
		addressModeW = SAMPLER_ADDRESS_MODE_REPEAT;
		mipLodBias = 0.0f;
		anisotropyEnable = false;
		maxAnisotropy = 0.0f;
		compareEnable = false;
		compareOp = COMPARE_OP_NEVER;
		minLod = 0.0f;
		maxLod = 0.0f;
		//borderColor = ;
		unnormalizedCoordinates = false;
	}

	Filter magFilter;
	Filter minFilter;
	//SamplerMipmapMode mipmapMode;
	SamplerAddressMode addressModeU;
	SamplerAddressMode addressModeV;
	SamplerAddressMode addressModeW;
	float mipLodBias;
	bool anisotropyEnable;
	float maxAnisotropy;
	bool compareEnable;
	CompareOp compareOp;
	float minLod;
	float maxLod;
	//BorderColor borderColor;
	bool unnormalizedCoordinates;
};

class Sampler
{
public:

	friend class CommandBuffer;

	//
	// Constructors / Destructor
	Sampler			(void);
	Sampler			(const SamplerCreateInfo & createInfo);
	~Sampler		(void);

public:

#if HAVE_OPENGL
	GLuint m_uSamplerObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkSampler m_sampler;
#endif // HAVE_VULKAN
};

}

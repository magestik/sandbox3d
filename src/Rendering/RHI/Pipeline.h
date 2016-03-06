#pragma once

namespace RHI
{

class Pipeline
{
public:

	friend class CommandBuffer;

	struct InputAssemblyState
	{
		InputAssemblyState(void) : topology(PRIMITIVE_POINT_LIST), enableRestartPrimitive(false) { /* ... */ }
		PrimitiveType topology;
		bool enableRestartPrimitive;
	};

	struct DepthState
	{
		DepthState(void)
		{
			enableWrite = true;
			compareOp = COMPARE_OP_LESS;
		}

		bool		enableWrite;
		CompareOp	compareOp;
	};

	struct StencilState
	{
		StencilState(void)
		{
			failOp = STENCIL_OP_ZERO;
			passOp = STENCIL_OP_ZERO;
			depthFailOp = STENCIL_OP_ZERO;
			compareOp = COMPARE_OP_ALWAYS;
			compareMask = UINT32_MAX;
			writeMask = 0;
			reference = 0;
		}

		StencilOp		failOp;
		StencilOp		passOp;
		StencilOp		depthFailOp;
		CompareOp		compareOp;
		unsigned int	compareMask;
		unsigned		writeMask;
		unsigned		reference;
	};

	struct DepthStencilState
	{
		DepthStencilState(void)
		{
			enableDepth = false;
			enableStencil = false;
		}

		bool			enableDepth;
		DepthState		depthState;

		bool			enableStencil;
		StencilState	frontStencilState;
		StencilState	backStencilState;
	};

	struct BlendState
	{
		BlendState(void)
		{
			enable = false;

			srcColorFactor	= srcAlphaFactor = BLEND_FACTOR_ONE;
			dstColorFactor	= dstAlphaFactor = BLEND_FACTOR_ZERO;
			colorEquation	= alphaEquation  = BLEND_OP_ADD;

			writeMask = COLOR_MASK_R | COLOR_MASK_G | COLOR_MASK_B | COLOR_MASK_A;
		}

		bool enable;

		BlendFactor srcColorFactor;
		BlendFactor dstColorFactor;
		BlendOp colorEquation;

		BlendFactor srcAlphaFactor;
		BlendFactor dstAlphaFactor;
		BlendOp alphaEquation;

		unsigned int writeMask;
	};

	//
	// Constructors / Destructor
	Pipeline		(void);
	Pipeline		(const InputAssemblyState & input, const DepthStencilState & depthStencil, const BlendState & blend);
	~Pipeline		(void);

public:

#if HAVE_OPENGL
	InputAssemblyState m_inputAssemblyState;
	DepthStencilState m_depthStencilState;
	BlendState m_blendState;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkPipeline m_pipeline;
#endif // HAVE_VULKAN
};

}

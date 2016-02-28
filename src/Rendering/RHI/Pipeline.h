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

	struct DepthStencilState
	{
		DepthStencilState(void)
		{
			enableDepthTest = false;
			enableDepthWrite = false;
			depthCompareOp = COMPARE_OP_NEVER;

			enableStencilTest = false;
			//mask	= UINT32_MAX;
			//func	= GL_ALWAYS;
		}

		bool		enableDepthTest;
		bool		enableDepthWrite;
		CompareOp	depthCompareOp;

		bool enableStencilTest;
		//GLuint mask;
		//GLenum func;
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

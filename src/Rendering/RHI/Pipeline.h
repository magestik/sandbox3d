#pragma once

#include <vector>

namespace RHI
{

class Pipeline
{
public:

	friend class CommandBuffer;

	struct InputAssemblyState
	{
		InputAssemblyState(void)
		{
			topology = PRIMITIVE_POINT_LIST;
			enableRestartPrimitive = false;
		}

		PrimitiveType topology;
		bool enableRestartPrimitive;
	};

	struct RasterizationState
	{
		RasterizationState(void)
		{
			enableDepthClamp = false;
			enableRasterizerDiscard = false;
			polygonMode = POLYGON_MODE_FILL;
			cullMode = CULL_MODE_NONE;
			frontFace = FRONT_FACE_COUNTER_CLOCKWISE;
			enableDepthBias = false;
			depthBiasConstantFactor = 0.0f;
			depthBiasClamp = 0.0f;
			depthBiasSlopeFactor = 0.0f;
			lineWidth = 1.0f;
		}

		bool			enableDepthClamp;
		bool			enableRasterizerDiscard;
		PolygonMode		polygonMode;
		CullMode		cullMode;
		FrontFace		frontFace;
		bool			enableDepthBias;
		float			depthBiasConstantFactor;
		float			depthBiasClamp;
		float			depthBiasSlopeFactor;
		float			lineWidth;
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

#if HAVE_OPENGL
	typedef GLuint ShaderModule;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	typedef VkShaderModule ShaderModule;
#endif // HAVE_VULKAN

	struct ShaderStage
	{
		ShaderStage(void)
		{
			stage = ShaderStageFlag(0);
			module = 0;
		}

		ShaderStageFlag stage;
		ShaderModule module;
	};

	//
	// Constructors / Destructor
	Pipeline		(void);
	Pipeline		(const InputAssemblyState & input, const RasterizationState & rasterization, const DepthStencilState & depthStencil, const BlendState & blend, const std::vector<ShaderStage> & aStages);
	~Pipeline		(void);

public:

#if HAVE_OPENGL
	InputAssemblyState m_inputAssemblyState;
	RasterizationState m_rasterizationState;
	DepthStencilState m_depthStencilState;
	BlendState m_blendState;
	GLuint m_uShaderObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkPipeline m_pipeline;
#endif // HAVE_VULKAN
};

}

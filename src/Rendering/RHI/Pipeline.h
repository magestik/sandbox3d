#pragma once

#include <vector>

namespace RHI
{

struct PipelineInputAssemblyStateCreateInfo
{
	PipelineInputAssemblyStateCreateInfo(void)
	{
		topology = PRIMITIVE_POINT_LIST;
		enableRestartPrimitive = false;
	}

	PrimitiveType topology;
	bool enableRestartPrimitive;
};

struct PipelineRasterizationStateCreateInfo
{
	PipelineRasterizationStateCreateInfo(void)
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

struct PipelineDepthStencilStateCreateInfo
{
	PipelineDepthStencilStateCreateInfo(void)
	{
		enableDepth = false;
		enableStencil = false;
	}

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

	bool			enableDepth;
	DepthState		depthState;

	bool			enableStencil;
	StencilState	frontStencilState;
	StencilState	backStencilState;
};

struct PipelineBlendStateCreateInfo
{
	PipelineBlendStateCreateInfo(void)
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

struct PipelineShaderStageCreateInfo
{
	PipelineShaderStageCreateInfo(void)
	{
		stage = ShaderStage(0);
		module = ShaderModule();
	}

	ShaderStage stage;
	ShaderModule module;
};

class Pipeline
{
public:

	friend class CommandBuffer;


	//
	// Constructors / Destructor
	Pipeline		(void);
	Pipeline		(const PipelineInputAssemblyStateCreateInfo & input, const PipelineRasterizationStateCreateInfo & rasterization, const PipelineDepthStencilStateCreateInfo & depthStencil, const PipelineBlendStateCreateInfo & blend, const std::vector<PipelineShaderStageCreateInfo> & aStages);
	~Pipeline		(void);

public:

#if HAVE_OPENGL
	PipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
	PipelineRasterizationStateCreateInfo m_rasterizationState;
	PipelineDepthStencilStateCreateInfo m_depthStencilState;
	PipelineBlendStateCreateInfo m_blendState;
	GLuint m_uShaderObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkPipeline m_pipeline;
#endif // HAVE_VULKAN
};

}

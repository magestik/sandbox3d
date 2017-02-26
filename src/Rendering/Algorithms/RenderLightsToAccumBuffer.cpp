#include "RenderLightsToAccumBuffer.h"

const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

/**
 * @brief Constructor
 * @param rendering
 */
RenderLightsToAccumBuffer::RenderLightsToAccumBuffer(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
RenderLightsToAccumBuffer::~RenderLightsToAccumBuffer(void)
{
	// ...
}

/**
 * @brief RenderLightsToAccumBuffer::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * RenderLightsToAccumBuffer::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new RenderLightsToAccumBuffer(rendering, framebuffer));
}

/**
 * @brief RenderLightsToAccumBufferAlgorithm::init
 * @return
 */
bool RenderLightsToAccumBuffer::init(void)
{
	//
	// Initialize Render Pass
	{
		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0; // disable depth buffer
		desc.aColorAttachments.push_back(0);
		desc.aColorAttachments.push_back(1);

		m_renderPass = RHI::RenderPass(desc);
	}

	//
	// Initialize Pipelines
	{
		RHI::PipelineInputAssemblyStateCreateInfo input;
		RHI::PipelineRasterizationStateCreateInfo rasterization;
		RHI::PipelineDepthStencilStateCreateInfo depthStencil;

		RHI::PipelineBlendStateCreateInfo blend;
		blend.enable = true;
		blend.colorEquation = RHI::BLEND_OP_ADD;
		blend.srcColorFactor = RHI::BLEND_FACTOR_ONE;
		blend.dstColorFactor = RHI::BLEND_FACTOR_ONE;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["directionnal_light.vert"]->GetObject();

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["directionnal_light.frag"]->GetObject();

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipelineDirectionalLight = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo depthSampler;
		depthSampler.minFilter = RHI::FILTER_NEAREST;
		depthSampler.magFilter = RHI::FILTER_NEAREST;
		depthSampler.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		depthSampler.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_samplerDepth = RHI::Sampler(depthSampler);

		RHI::SamplerCreateInfo normalSampler;
		normalSampler.minFilter = RHI::FILTER_NEAREST;
		normalSampler.magFilter = RHI::FILTER_NEAREST;
		normalSampler.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		normalSampler.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		m_samplerNormal = RHI::Sampler(normalSampler);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipelineDirectionalLight.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipelineDirectionalLight.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief RenderLightsToAccumBufferAlgorithm::render
 * @param commandBuffer
 * @return
 */
bool RenderLightsToAccumBuffer::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), vec4(0.0f, 0.0f, 0.0f, 0.0f));

	{
		commandBuffer.Bind(m_pipelineDirectionalLight);

		mat4x4 mCameraViewProjection = m_rendering.m_matProjection * m_rendering.m_matCurrentView;

		SetUniform(m_pipelineDirectionalLight.m_uShaderObject, "viewPos", (inverse(m_rendering.m_matCurrentView) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
		SetUniform(m_pipelineDirectionalLight.m_uShaderObject, "InverseViewProjection", inverse(mCameraViewProjection));
		SetUniform(m_pipelineDirectionalLight.m_uShaderObject, "lightDir", - normalize(m_rendering.m_pLight->GetDirection()));
		SetUniform(m_pipelineDirectionalLight.m_uShaderObject, "lightColor", sRGB_to_XYZ * m_rendering.m_pLight->GetColor());

		SetTexture(m_pipelineDirectionalLight.m_uShaderObject, "depthSampler", 0, *(m_rendering.m_mapTargets["depth"].getTexture()), m_samplerDepth);
		SetTexture(m_pipelineDirectionalLight.m_uShaderObject, "normalSampler", 1, *(m_rendering.m_mapTargets["normals"].getTexture()), m_samplerNormal);

		m_rendering.m_pQuadMesh->draw(commandBuffer);

		// TODO : render all lights
	}

	commandBuffer.EndRenderPass();

	return(true);
}

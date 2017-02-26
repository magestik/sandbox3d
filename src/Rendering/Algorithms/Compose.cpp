#include "Compose.h"

#include "../utils.inl"

// sRGB
const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

// Adobe RGB
const mat3x3 RGB_to_XYZ(0.5767309, 0.1855540, 0.1881852, 0.2973769, 0.6273491, 0.0752741, 0.0270343, 0.0706872, 0.9911085);
const mat3x3 XYZ_to_RGB(2.0413690, -0.5649464, -0.3446944, -0.9692660, 1.8760108, 0.0415560, 0.0134474, -0.1183897, 1.0154096);

/**
 * @brief Constructor
 * @param rendering
 */
Compose::Compose(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
Compose::~Compose(void)
{
	// ...
}

/**
 * @brief Compose::Create
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * Compose::Create(Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	return(new Compose(rendering, framebuffer));
}

/**
 * @brief ComposeAlgorithm::init
 * @return
 */
bool Compose::init(void)
{
	//
	// Initialize Render Pass
	{
		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0; // enable depth buffer for early-Z culling
		desc.aColorAttachments.push_back(0);

		m_renderPass = RHI::RenderPass(desc);
	}

	//
	// Initialize Pipelines
	{
		RHI::PipelineInputAssemblyStateCreateInfo input;
		RHI::PipelineRasterizationStateCreateInfo rasterization;

		RHI::PipelineDepthStencilStateCreateInfo depthStencil;
		depthStencil.enableDepth = true;
		depthStencil.depthState.compareOp = RHI::COMPARE_OP_EQUAL;
		depthStencil.depthState.enableWrite = false;

		RHI::PipelineBlendStateCreateInfo blend;

		RHI::PipelineShaderStageCreateInfo vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = m_rendering.m_mapShaderModules["full.vert"];

		RHI::PipelineShaderStageCreateInfo fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = m_rendering.m_mapShaderModules["full.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
	}

	//
	// Initialize Samplers
	{
		RHI::SamplerCreateInfo samplerLinear;
		samplerLinear.minFilter = RHI::FILTER_LINEAR;
		samplerLinear.magFilter = RHI::FILTER_LINEAR;
		samplerLinear.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerLinear.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		RHI::SamplerCreateInfo samplerNearest;
		samplerNearest.minFilter = RHI::FILTER_NEAREST;
		samplerNearest.magFilter = RHI::FILTER_NEAREST;
		samplerNearest.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerNearest.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		RHI::SamplerCreateInfo samplerShadowMap;
		samplerShadowMap.minFilter = RHI::FILTER_LINEAR;
		samplerShadowMap.magFilter = RHI::FILTER_LINEAR;
		samplerShadowMap.addressModeU = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerShadowMap.addressModeV = RHI::SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerShadowMap.compareEnable = true;
		samplerShadowMap.compareOp = RHI::COMPARE_OP_LESS_OR_EQUAL;

		m_samplerDiffuseSampler = RHI::Sampler(samplerLinear);
		m_samplerSpecularSampler = RHI::Sampler(samplerLinear);

		m_samplerDiffuseLightSampler = RHI::Sampler(samplerNearest);
		m_samplerSpecularLightSampler = RHI::Sampler(samplerNearest);

		m_samplerShadowMap = RHI::Sampler(samplerShadowMap);
	}

	//
	// TODO : remove this
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	return(true);
}

/**
 * @brief ComposeAlgorithm::render
 * @param commandBuffer
 * @return
 */
bool Compose::render(RHI::CommandBuffer & commandBuffer)
{
	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), m_rendering.m_vCurrentClearColor);

	{
		commandBuffer.Bind(m_pipeline);

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_rendering.m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_rendering.m_matShadowMapProjection * mDepthView;

		SetTexture(m_pipeline.m_uShaderObject, "diffuseLightSampler", 0, *(m_rendering.m_mapTargets["lights_diffuse"].getTexture()), m_samplerDiffuseLightSampler);
		SetTexture(m_pipeline.m_uShaderObject, "specularLightSampler", 1, *(m_rendering.m_mapTargets["lights_specular"].getTexture()), m_samplerSpecularLightSampler);
		SetTexture(m_pipeline.m_uShaderObject, "shadowMap", 2, *(m_rendering.m_mapTargets["shadow_map"].getTexture()), m_samplerShadowMap);

		SetUniform(m_pipeline.m_uShaderObject, "ambientColor", RGB_to_XYZ * m_rendering.environment.ambient.Color);
		SetUniform(m_pipeline.m_uShaderObject, "DepthTransformation", mDepthViewProjection);
		SetUniform(m_pipeline.m_uShaderObject, "View", m_rendering.m_matCurrentView);

		unsigned int offset = 0;

		for (Mesh::Instance & object : m_rendering.m_aObjects)
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

			object.mesh->bind();

			for (SubMesh * m : object.getDrawCommands())
			{
				SetTexture(m_pipeline.m_uShaderObject, "diffuseSampler", 3, *(m->m_material.m_diffuse), m_samplerDiffuseSampler);
				SetTexture(m_pipeline.m_uShaderObject, "specularSampler", 4, *(m->m_material.m_specular), m_samplerSpecularSampler);

				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++offset;
		}
	}

	commandBuffer.EndRenderPass();

	return(true);
}

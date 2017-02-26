#include "Compose.h"

#include "../utils.inl"

const mat3x3 sRGB_to_XYZ(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

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
		RHI::Pipeline::InputAssemblyState input;
		RHI::Pipeline::RasterizationState rasterization;

		RHI::Pipeline::DepthStencilState depthStencil;
		depthStencil.enableDepth = true;
		depthStencil.depthState.compareOp = RHI::COMPARE_OP_EQUAL;
		depthStencil.depthState.enableWrite = false;

		RHI::Pipeline::BlendState blend;

		RHI::Pipeline::ShaderStage vertexShader;
		vertexShader.stage = RHI::SHADER_STAGE_VERTEX;
		vertexShader.module = g_VertexShaders["full.vert"]->GetObject();

		RHI::Pipeline::ShaderStage fragmentShader;
		fragmentShader.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader.module = g_FragmentShaders["full.frag"]->GetObject();

		std::vector<RHI::Pipeline::ShaderStage> aStages;
		aStages.push_back(vertexShader);
		aStages.push_back(fragmentShader);

		m_pipeline = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages);
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

		SetTexture(m_pipeline.m_uShaderObject, "diffuseLightSampler", 0, *(m_rendering.m_mapTargets["lights_diffuse"].getTexture()), m_rendering.GetPipeline("compose")->m_mapSamplers["diffuseLightSampler"]);
		SetTexture(m_pipeline.m_uShaderObject, "specularLightSampler", 1, *(m_rendering.m_mapTargets["lights_specular"].getTexture()), m_rendering.GetPipeline("compose")->m_mapSamplers["specularLightSampler"]);
		SetTexture(m_pipeline.m_uShaderObject, "shadowMap", 2, *(m_rendering.m_mapTargets["shadow_map"].getTexture()), m_rendering.GetPipeline("compose")->m_mapSamplers["shadowMap"]);

		SetUniform(m_pipeline.m_uShaderObject, "ambientColor", sRGB_to_XYZ * m_rendering.environment.ambient.Color);
		SetUniform(m_pipeline.m_uShaderObject, "DepthTransformation", mDepthViewProjection);
		SetUniform(m_pipeline.m_uShaderObject, "View", m_rendering.m_matCurrentView);

		unsigned int offset = 0;

		for (Mesh::Instance & object : m_rendering.m_aObjects)
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

			object.mesh->bind();

			for (SubMesh * m : object.getDrawCommands())
			{
				SetTexture(m_pipeline.m_uShaderObject, "diffuseSampler", 3, *(m->m_material.m_diffuse), m_rendering.GetPipeline("compose")->m_mapSamplers["diffuseSampler"]);
				SetTexture(m_pipeline.m_uShaderObject, "specularSampler", 4, *(m->m_material.m_specular), m_rendering.GetPipeline("compose")->m_mapSamplers["specularSampler"]);

				m->draw(commandBuffer);
			}

			object.mesh->unbind();

			++offset;
		}
	}

	commandBuffer.EndRenderPass();

	return(true);
}

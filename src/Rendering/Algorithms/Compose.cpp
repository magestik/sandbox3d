#include "Compose.h"

#include <string.h>

#include "../utils.inl"

#include "Remotery.h"

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
Compose::Compose(Rendering & rendering, RHI::Framebuffer & framebuffer) : GraphicsAlgorithm(rendering, framebuffer), m_pDepthTexture(nullptr), m_pDiffuseLightsTexture(nullptr), m_pSpecularLightsTexture(nullptr)
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

		RHI::PipelineShaderStageCreateInfo fragmentShader_variant_diffuse_specular;
		fragmentShader_variant_diffuse_specular.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader_variant_diffuse_specular.module = m_rendering.m_mapShaderModules["full_variant_diffuse_specular.frag"];

		RHI::PipelineShaderStageCreateInfo fragmentShader_variant_diffuse_only;
		fragmentShader_variant_diffuse_only.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader_variant_diffuse_only.module = m_rendering.m_mapShaderModules["full_variant_diffuse_only.frag"];

		RHI::PipelineShaderStageCreateInfo fragmentShader_variant_specular_only;
		fragmentShader_variant_specular_only.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader_variant_specular_only.module = m_rendering.m_mapShaderModules["full_variant_specular_only.frag"];

		RHI::PipelineShaderStageCreateInfo fragmentShader_variant_none;
		fragmentShader_variant_none.stage = RHI::SHADER_STAGE_FRAGMENT;
		fragmentShader_variant_none.module = m_rendering.m_mapShaderModules["full_variant_none.frag"];

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages_variant_diffuse_specular;
		aStages_variant_diffuse_specular.push_back(vertexShader);
		aStages_variant_diffuse_specular.push_back(fragmentShader_variant_diffuse_specular);

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages_variant_diffuse_only;
		aStages_variant_diffuse_only.push_back(vertexShader);
		aStages_variant_diffuse_only.push_back(fragmentShader_variant_diffuse_only);

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages_variant_specular_only;
		aStages_variant_specular_only.push_back(vertexShader);
		aStages_variant_specular_only.push_back(fragmentShader_variant_specular_only);

		std::vector<RHI::PipelineShaderStageCreateInfo> aStages_variant_none;
		aStages_variant_none.push_back(vertexShader);
		aStages_variant_none.push_back(fragmentShader_variant_none);

		m_pipeline_diffuse_specular = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages_variant_diffuse_specular);
		m_pipeline_diffuse_only = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages_variant_diffuse_only);
		m_pipeline_specular_only = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages_variant_specular_only);
		m_pipeline_none = RHI::Pipeline(input, rasterization, depthStencil, blend, aStages_variant_none);
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
	SetUniformBlockBinding(m_pipeline_diffuse_specular.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline_diffuse_specular.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);
	SetUniformBlockBinding(m_pipeline_diffuse_only.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline_diffuse_only.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);
	SetUniformBlockBinding(m_pipeline_specular_only.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline_specular_only.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);
	SetUniformBlockBinding(m_pipeline_none.m_uShaderObject, "CameraBlock", Rendering::BLOCK_BINDING_CAMERA);
	SetUniformBlockBinding(m_pipeline_none.m_uShaderObject, "ObjectBlock", Rendering::BLOCK_BINDING_OBJECT);

	//
	//
#if HAVE_OPENGL // ugly but needed for now (quick prototyping)
	if (nullptr != m_pDepthTexture)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer.m_uFramebufferObject);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pDepthTexture->GetObject(), 0);

		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		assert(GL_FRAMEBUFFER_COMPLETE == status);
	}
#endif // HAVE_OPENGL

	return(true);
}

/**
 * @brief Compose::release
 * @return
 */
bool Compose::release(void)
{
	return(false); // TODO
}

/**
 * @brief Compose::render
 * @param commandBuffer
 * @return
 */
bool Compose::render(RHI::CommandBuffer & commandBuffer)
{
	rmt_ScopedOpenGLSample(Compose);

	commandBuffer.BeginRenderPass(m_renderPass, m_framebuffer, ivec2(0, 0), ivec2(m_rendering.GetWidth(), m_rendering.GetHeight()), m_rendering.m_vCurrentClearColor);

	{
		commandBuffer.Bind(m_pipeline_diffuse_specular);

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_rendering.GetScene().m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_rendering.m_matShadowMapProjection * mDepthView;

		if (m_pDiffuseLightsTexture)
		{
			SetTexture(m_pipeline_diffuse_specular.m_uShaderObject, "diffuseLightSampler", 0, *m_pDiffuseLightsTexture, m_samplerDiffuseLightSampler);
		}

		if (m_pSpecularLightsTexture)
		{
			SetTexture(m_pipeline_diffuse_specular.m_uShaderObject, "specularLightSampler", 1, *m_pSpecularLightsTexture, m_samplerSpecularLightSampler);
		}

		const GPU::Texture<GL_TEXTURE_2D> * pShadowMap = m_rendering.GetRenderTexture("shadow_map");
		if (pShadowMap)
		{
			SetTexture(m_pipeline_diffuse_specular.m_uShaderObject, "shadowMap", 2, *pShadowMap, m_samplerShadowMap);
		}

		SetUniform(m_pipeline_diffuse_specular.m_uShaderObject, "ambientColor", RGB_to_XYZ * m_rendering.environment.ambient.Color);
		SetUniform(m_pipeline_diffuse_specular.m_uShaderObject, "DepthTransformation", mDepthViewProjection);
		SetUniform(m_pipeline_diffuse_specular.m_uShaderObject, "View", m_rendering.m_matCurrentView);

		unsigned int offset = 0;

		for (const Object & object : m_rendering.GetScene().getObjects())
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

			for (const Object::Mesh & mesh : object.Meshes)
			{
				if (mesh.DiffuseMapID != 0 && mesh.SpecularMapID != 0)
			    {
					GLuint DiffuseMapId = m_rendering.GetTexture(mesh.DiffuseMapID);
					SetTexture<GL_TEXTURE_2D>(m_pipeline_diffuse_specular.m_uShaderObject, "diffuseSampler", 3, DiffuseMapId, m_samplerDiffuseSampler);

					GLuint SpecualMapId = m_rendering.GetTexture(mesh.SpecularMapID);
					SetTexture<GL_TEXTURE_2D>(m_pipeline_diffuse_specular.m_uShaderObject, "specularSampler", 4, SpecualMapId, m_samplerSpecularSampler);

					Mesh * pRenderingMesh = m_rendering.GetMesh(mesh.MeshID);

					pRenderingMesh->bind();

					for (SubMesh * m : pRenderingMesh->m_aSubMeshes)
					{
						m->draw(commandBuffer);
					}

					pRenderingMesh->unbind();
				}
			}

			++offset;
		}
	}

	{
		commandBuffer.Bind(m_pipeline_none);

		mat4x4 mDepthView = _lookAt(vec3(0,0,0), m_rendering.GetScene().m_pLight->GetDirection(), vec3(0.0f, -1.0f, 0.0f));
		mat4x4 mDepthViewProjection = m_rendering.m_matShadowMapProjection * mDepthView;

		if (m_pDiffuseLightsTexture)
		{
			SetTexture(m_pipeline_none.m_uShaderObject, "diffuseLightSampler", 0, *m_pDiffuseLightsTexture, m_samplerDiffuseLightSampler);
		}

		if (m_pSpecularLightsTexture)
		{
			SetTexture(m_pipeline_none.m_uShaderObject, "specularLightSampler", 1, *m_pSpecularLightsTexture, m_samplerSpecularLightSampler);
		}

		const GPU::Texture<GL_TEXTURE_2D> * pShadowMap = m_rendering.GetRenderTexture("shadow_map");
		if (pShadowMap)
		{
			SetTexture(m_pipeline_none.m_uShaderObject, "shadowMap", 2, *pShadowMap, m_samplerShadowMap);
		}

		SetUniform(m_pipeline_none.m_uShaderObject, "ambientColor", RGB_to_XYZ * m_rendering.environment.ambient.Color);
		SetUniform(m_pipeline_none.m_uShaderObject, "DepthTransformation", mDepthViewProjection);
		SetUniform(m_pipeline_none.m_uShaderObject, "View", m_rendering.m_matCurrentView);

		unsigned int offset = 0;

		for (const Object & object : m_rendering.GetScene().getObjects())
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Rendering::BLOCK_BINDING_OBJECT, m_rendering.m_pObjectsBuffer->GetObject(), sizeof(Rendering::ObjectBlock)*offset, sizeof(Rendering::ObjectBlock));

			for (const Object::Mesh & mesh : object.Meshes)
			{
				if (mesh.DiffuseMapID == 0 && mesh.SpecularMapID == 0)
				{
					Mesh * pRenderingMesh = m_rendering.GetMesh(mesh.MeshID);

					SetUniform(m_pipeline_none.m_uShaderObject, "diffuseColor", mesh.Kd);
					SetUniform(m_pipeline_none.m_uShaderObject, "specularColor", mesh.Ks);

					pRenderingMesh->bind();

					for (SubMesh * m : pRenderingMesh->m_aSubMeshes)
					{
						m->draw(commandBuffer);
					}

					pRenderingMesh->unbind();
				}
			}

			++offset;
		}
	}

	commandBuffer.EndRenderPass();

	return(true);
}

/**
 * @brief Compose::setParameter
 * @param name
 * @param value
 */
void Compose::setParameter(const char * name, const char * value)
{
	if (!strcmp("depth", name))
	{
		m_pDepthTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else if (!strcmp("diffuse_lights", name))
	{
		m_pDiffuseLightsTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else if (!strcmp("specular_lights", name))
	{
		m_pSpecularLightsTexture = m_rendering.m_mapTargets[value].getTexture();
	}
	else
	{
		assert(false);
	}
}

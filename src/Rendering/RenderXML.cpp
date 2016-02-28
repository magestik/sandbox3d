#include "RenderXML.h"

#include <assert.h>

#include "StrToGL.h"

#include <tinyxml2.h>

using namespace tinyxml2;

//
// String for parsing

static const char TARGETS_STR []		= "targets";
static const char TARGET_STR []			= "texture";

static const char FRAMEBUFFERS_STR []	= "framebuffers";
static const char FRAMEBUFFER_STR []	= "framebuffer";

static const char PIPELINES_STR []		= "pipelines";
static const char PIPELINE_STR []		= "pipeline";

static const char PASSES_STR []			= "pass_list";
static const char PASS_STR []			= "pass";

/**
 * @brief Constructor
 */
RenderXML::RenderXML(const char * filename)
{
	XMLDocument * doc = new XMLDocument;

	if (!doc)
	{
		return;
	}

	XMLError err = doc->LoadFile(filename);

	if (XML_SUCCESS != err)
	{
		delete doc;
		return;
	}

	m_pData = (void*)doc;

}

/**
 * @brief Destructor
 */
RenderXML::~RenderXML(void)
{
	delete((XMLDocument*)m_pData);
}

/**
 * @brief RenderXML::initializePipelines
 * @param rendering
 */
void RenderXML::initializePipelines(Rendering & rendering)
{
	const XMLElement * root = static_cast<XMLDocument*>(m_pData)->RootElement();
	assert(nullptr != root);

	const XMLElement * pipelines = root->FirstChildElement(PIPELINES_STR);
	assert(nullptr != pipelines);

	const XMLElement * elmt = pipelines->FirstChildElement(PIPELINE_STR);

	while (nullptr != elmt)
	{
		const char * name = elmt->Attribute("name");

		assert(nullptr != name);

		rendering.m_mapPipeline[name] = new Pipeline(elmt, rendering);

		elmt = elmt->NextSiblingElement(PIPELINE_STR);
	}
}

/**
 * @brief RenderXML::initializeTargets
 * @param rendering
 */
void RenderXML::initializeTargets(Rendering & rendering)
{
	const XMLElement * root = static_cast<XMLDocument*>(m_pData)->RootElement();
	assert(nullptr != root);

	const XMLElement * targets = root->FirstChildElement(TARGETS_STR);
	assert(nullptr != targets);

	const XMLElement * elmt = targets->FirstChildElement(TARGET_STR);

	while (nullptr != elmt)
	{
		const char * name = elmt->Attribute("name");

		const char * format = elmt->Attribute("format");

		unsigned int size_divisor = elmt->UnsignedAttribute("size_divisor");

		if (0 == size_divisor)
		{
			size_divisor = 1;
		}

		RenderTexture RT(strToFormat(format), size_divisor);

		rendering.m_mapTargets.insert(std::pair<std::string, RenderTexture>(name, RT));

		elmt = elmt->NextSiblingElement("texture");
	}
}

/**
 * @brief RenderXML::initializePasses
 * @param rendering
 */
void RenderXML::initializePasses(Rendering & rendering)
{
	const XMLElement * root = static_cast<XMLDocument*>(m_pData)->RootElement();
	assert(nullptr != root);

	const XMLElement * passes = root->FirstChildElement(PASSES_STR);
	assert(nullptr != passes);

	const XMLElement * elmt = passes->FirstChildElement(PASS_STR);

	while (nullptr != elmt)
	{
		createPass(elmt, rendering);

		elmt = elmt->NextSiblingElement(PASS_STR);
	}
}

/**
 * @brief RenderXML::initializeFramebuffers
 * @param rendering
 */
void RenderXML::initializeFramebuffers(Rendering & rendering)
{
	const XMLElement * root = static_cast<XMLDocument*>(m_pData)->RootElement();
	assert(nullptr != root);

	const XMLElement * framebuffers = root->FirstChildElement(FRAMEBUFFERS_STR);
	assert(nullptr != framebuffers);

	const XMLElement * elmt = framebuffers->FirstChildElement(FRAMEBUFFER_STR);

	while (nullptr != elmt)
	{
		createFramebuffer(elmt, rendering);

		elmt = elmt->NextSiblingElement(FRAMEBUFFER_STR);
	}
}

/**
 * @brief RenderXML::createPass
 * @param pNode
 * @param rendering
 */
void RenderXML::createPass(const void * pNode, Rendering & rendering)
{
	std::vector<RHI::RenderPass::SubpassDescription> aSubpass;

	const char * passName = static_cast<const XMLElement*>(pNode)->Attribute("name");

	const XMLElement * elmt = static_cast<const XMLElement*>(pNode)->FirstChildElement("subpass");

	while (nullptr != elmt)
	{
		const char * name = elmt->Attribute("name");
		assert(nullptr != name);

		RHI::RenderPass::SubpassDescription desc;
		desc.depthAttachment = 0;

		getSubpassDescription(elmt, desc);

		aSubpass.push_back(desc);

		elmt = elmt->NextSiblingElement("subpass");
	}

	rendering.m_mapPass[passName] = RHI::RenderPass(aSubpass);
}

/**
 * @brief RenderXML::createFramebuffer
 * @param pNode
 * @param rendering
 */
void RenderXML::createFramebuffer(const void * pNode, Rendering & rendering)
{
	std::vector<const GPU::Texture<GL_TEXTURE_2D> *> aTextures;

	const char * framebufferName = static_cast<const XMLElement*>(pNode)->Attribute("name");

	const XMLElement * elmt = static_cast<const XMLElement*>(pNode)->FirstChildElement("output");

	while (NULL != elmt)
	{
		const char * texture = elmt->Attribute("texture");
		assert(nullptr != texture);

		const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = rendering.GetRenderTexture(texture);

		aTextures.push_back(pRenderTexture);

		elmt = elmt->NextSiblingElement("output");
	}

	rendering.m_mapFramebuffer[framebufferName] = RHI::Framebuffer(aTextures);
}

/**
 * @brief RenderXML::getSubpassDescription
 * @param pNode
 * @param subpassDesc
 */
void RenderXML::getSubpassDescription(const void * pNode, RHI::RenderPass::SubpassDescription & subpassDesc)
{
	const XMLElement * elmt = static_cast<const XMLElement*>(pNode)->FirstChildElement("output");

	while (nullptr != elmt)
	{
		const char * buffer = elmt->Attribute("color_buffer");

		if (nullptr != buffer)
		{
			subpassDesc.aColorAttachments.push_back(atoi(buffer));
		}

		elmt = elmt->NextSiblingElement("output");
	}
}

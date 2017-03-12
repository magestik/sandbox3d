#include "RenderXML.h"

#include <assert.h>

#include "StrToGL.h"

#include <tinyxml2.h>

using namespace tinyxml2;

//
// String for parsing

static const char TARGETS_STR []			= "targets";
static const char TARGET_STR []				= "texture";

static const char FRAMEBUFFERS_STR []		= "framebuffers";
static const char FRAMEBUFFER_STR []		= "framebuffer";

static const char QUEUE_STR []				= "queue";
static const char QUEUE_ELMT_STR []			= "element";
static const char QUEUE_ELMT_PARAM_STR []	= "param";

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

	if (XML_SUCCESS != doc->LoadFile(filename))
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
 * @brief RenderXML::initializeTargets
 * @param rendering
 */
void RenderXML::initializeTargets(Rendering & rendering)
{
	const XMLElement * root = static_cast<const XMLDocument*>(m_pData)->RootElement();
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
 * @brief RenderXML::initializeFramebuffers
 * @param rendering
 */
void RenderXML::initializeFramebuffers(Rendering & rendering)
{
	const XMLElement * root = static_cast<const XMLDocument*>(m_pData)->RootElement();
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
 * @brief RenderXML::initializeQueue
 * @param rendering
 */
void RenderXML::initializeQueue(Rendering & rendering)
{
	const XMLElement * root = static_cast<const XMLDocument*>(m_pData)->RootElement();
	assert(nullptr != root);

	const XMLElement * queue = root->FirstChildElement(QUEUE_STR);
	assert(nullptr != queue);

	const XMLElement * elmt = queue->FirstChildElement(QUEUE_ELMT_STR);

	while (nullptr != elmt)
	{
		const char * type = elmt->Attribute("type");

		const char * fb = elmt->Attribute("framebuffer");

		GraphicsAlgorithm * a = GraphicsAlgorithm::Create(type, rendering, rendering.m_mapFramebuffer[fb]);

		rendering.m_renderQueue.push_back(a);

		setParameters(elmt, a, rendering);

		elmt = elmt->NextSiblingElement(QUEUE_ELMT_STR);
	}
}

/**
 * @brief RenderXML::setParameters
 * @param pNode
 * @param pAlgo
 * @param rendering
 */
void RenderXML::setParameters(const void * pNode, GraphicsAlgorithm * pAlgo, Rendering & rendering)
{
	const XMLElement * root = static_cast<const XMLElement*>(pNode);
	assert(nullptr != root);

	const XMLElement * elmt = root->FirstChildElement(QUEUE_ELMT_PARAM_STR);

	while (nullptr != elmt)
	{
		const char * name = elmt->Attribute("name");

		const char * value = elmt->Attribute("value");

		pAlgo->setParameter(name, value);

		elmt = elmt->NextSiblingElement(QUEUE_ELMT_PARAM_STR);
	}
}

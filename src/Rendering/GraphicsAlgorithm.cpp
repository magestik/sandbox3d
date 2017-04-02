#include "GraphicsAlgorithm.h"

#include "Algorithms/RenderDepthOnly.h"
#include "Algorithms/RenderSceneToGBuffer.h"
#include "Algorithms/RenderSceneToShadowMap.h"
#include "Algorithms/RenderLightsToAccumBuffer.h"
#include "Algorithms/Compose.h"
#include "Algorithms/Fog.h"
#include "Algorithms/BrightFilter.h"
#include "Algorithms/ToneMapping.h"
#include "Algorithms/FXAA.h"
#include "Algorithms/SSAO.h"
#include "Algorithms/BlurH.h"
#include "Algorithms/BlurV.h"
#include "Algorithms/Bloom.h"
#include "Algorithms/SobelFilter.h"
#include "Algorithms/CelShading.h"

std::map<std::string, GraphicsAlgorithmFactory> GraphicsAlgorithm::m_FactoryMap;

/**
 * @brief Constructor
 */
GraphicsAlgorithm::GraphicsAlgorithm(Rendering & rendering, RHI::Framebuffer & framebuffer) :  m_rendering(rendering), m_framebuffer(framebuffer)
{
	// ...
}

/**
 * @brief Destructor
 */
GraphicsAlgorithm::~GraphicsAlgorithm()
{
	// ...
}

/**
 * @brief GraphicsAlgorithm::Create
 * @param szName
 * @param rendering
 * @param framebuffer
 * @return
 */
GraphicsAlgorithm * GraphicsAlgorithm::Create(const char * szType, Rendering & rendering, RHI::Framebuffer & framebuffer)
{
	GraphicsAlgorithmFactory factory = m_FactoryMap[szType];
	return(factory(rendering, framebuffer));
}

/**
 * @brief GraphicsAlgorithm::Register
 * @param szName
 * @param factory
 */
void GraphicsAlgorithm::RegisterEverything(void)
{
	m_FactoryMap["depth-only"]				= RenderDepthOnly::Create;
	m_FactoryMap["geometry"]				= RenderSceneToGBuffer::Create;
	m_FactoryMap["lights"]					= RenderLightsToAccumBuffer::Create;
	m_FactoryMap["shadowmap-directional"]	= RenderSceneToShadowMap::Create;

	m_FactoryMap["compose"]					= Bloom::Create;
	m_FactoryMap["BlurH"]					= BlurH::Create;
	m_FactoryMap["BlurV"]					= BlurV::Create;
	m_FactoryMap["bright"]					= BrightFilter::Create;
	m_FactoryMap["lighting"]				= Compose::Create;
	m_FactoryMap["fog"]						= Fog::Create;
	m_FactoryMap["FXAA"]					= FXAA::Create;
	m_FactoryMap["SSAO"]					= SSAO::Create;
	m_FactoryMap["sobel"]					= SobelFilter::Create;
	m_FactoryMap["tone-mapping"]			= ToneMapping::Create;
	m_FactoryMap["cel-shading"]				= CelShading::Create;
}

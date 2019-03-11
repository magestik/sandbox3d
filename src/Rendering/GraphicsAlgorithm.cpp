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

extern Rendering  * g_pRendering; // TODO : remove this

std::map<std::string, GraphicsAlgorithmFactory> GraphicsAlgorithm::m_FactoryMap;

/**
 * @brief Constructor
 */
GraphicsAlgorithm::GraphicsAlgorithm() :  m_rendering(*g_pRendering)
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
 * @brief GraphicsAlgorithm::render
 * @param parameters
 * @return
 */
bool GraphicsAlgorithm::render(const RenderGraph::Parameters & parameters)
{
	RHI::CommandBuffer commandBuffer;

	commandBuffer.Begin();

	render(parameters, commandBuffer);

	commandBuffer.End();

	return true;
}

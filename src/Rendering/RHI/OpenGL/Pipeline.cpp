#include "../RHI.h"

#if HAVE_OPENGL

/**
 * @brief Default constructor
 */
RHI::Pipeline::Pipeline(void)
: m_inputAssemblyState()
, m_depthStencilState()
, m_blendState()
{
	// ...
}

/**
 * @brief Constructor
 * @param input
 */
RHI::Pipeline::Pipeline(const InputAssemblyState & input, const DepthStencilState & depthStencil, const BlendState & blend)
: m_inputAssemblyState(input)
, m_depthStencilState(depthStencil)
, m_blendState(blend)
{
	// ...
}

/**
 * @brief Destructor
 */
RHI::Pipeline::~Pipeline(void)
{
	// ...
}

#endif // HAVE_OPENGL

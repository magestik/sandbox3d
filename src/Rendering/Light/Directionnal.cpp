#include "Light.h"

/**
 * @brief Light::Directionnal::Directionnal
 * @param direction
 */
Light::Directionnal::Directionnal(const vec3 & direction)
: m_vDirection(direction)
, m_vColor(1.0f, 1.0f, 1.0f)
{
	// ...
}

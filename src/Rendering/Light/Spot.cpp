#include "Light.h"


Light::Spot::Spot(const vec3 & position, const vec3 & direction, float angle)
: m_vPosition(position)
, m_vDirection(direction)
, m_fAngle(angle)
{

}

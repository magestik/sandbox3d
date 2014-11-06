#pragma once

#include <Vector.h>
#include <Matrix.h>

namespace Light
{

class Spot
{

public:

	Spot(const vec3 & position, const vec3 & direction, float angle);

	const vec3 & GetPosition(void) const { return(m_vPosition); }
	const vec3 & GetDirection(void) const { return(m_vDirection); }
	float GetAngle(void) const { return(m_fAngle); }

private:

	vec3 m_vPosition;
	vec3 m_vDirection;
	float m_fAngle;

};

}

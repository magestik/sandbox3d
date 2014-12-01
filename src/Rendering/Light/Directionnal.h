#pragma once

#include <Vector.h>
#include <Matrix.h>

namespace Light
{

class Directionnal
{
public:

	Directionnal(const vec3 & direction);

	const vec3 & GetDirection(void) const { return(m_vDirection); }

	const vec3 & GetColor(void) const { return(m_vColor); }

private:
	vec3 m_vDirection;

	vec3 m_vColor;
};

}

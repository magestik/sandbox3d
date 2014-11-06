#pragma once

namespace Light
{

class Omni
{

public:

	Omni(const vec3 & position);

	const vec3 & GetPosition(void) { return(m_vPosition); }

private:

	vec3 m_vPosition;

};

}

#pragma GLSL(version) 330

#include "../Interface.h"

uniform vec3 color;

out vec3 outColor;

void main()
{
	outColor = color;
}

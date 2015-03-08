#pragma GLSL(version) 330

#include "../Interface.h"

layout (location = POSITION) in vec3 inPosition;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0);
}

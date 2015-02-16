#pragma GLSL(version) 330

#include "../Interface.h"

layout (location = POSITION) in vec2 pos;

void main(void)
{
	gl_Position = vec4(pos, 0.0, 1.0);
}

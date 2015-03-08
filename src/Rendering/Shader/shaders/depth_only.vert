#pragma GLSL(version) 330

#include "../Interface.h"

uniform mat4x4 LightViewProjection;

uniform mat4x4 Model;

layout (location = POSITION) in vec3 pos;

void main(void)
{
	gl_Position = vec4(pos, 1.0) * Model * LightViewProjection;
}

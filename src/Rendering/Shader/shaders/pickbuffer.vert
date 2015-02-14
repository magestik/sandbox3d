#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	uint color;
};

uniform mat4x4 ViewProjection;
uniform mat4x4 Model;

layout (location = POSITION) in vec3 inPosition;
layout (location = COLOR) in uint inColor;

flat out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.color = inColor;

	gl_Position = vec4(inPosition, 1.0) * Model * ViewProjection;
}

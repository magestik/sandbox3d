#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	uint color;
};

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;
uniform uint id;

layout (location = POSITION) in vec3 inPosition;

flat out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.color = id;

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

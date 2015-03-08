#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec3 normal;
};

layout (std140) uniform CameraBlock
{
	mat4x4 ViewProjection;
};

uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 normal = vec4(inNormal, 0.0) * Model;

	vsOut.normal = normal.xyz;

	gl_Position = (vec4(inPosition, 1.0) * Model) * ViewProjection;
}

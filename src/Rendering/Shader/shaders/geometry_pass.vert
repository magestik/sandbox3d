#version 330 core

#include "../Interface.h"

#include "geometry_pass.h"

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;

layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlock);
layout (std140) uniform OBJECT_BLOCK_DEFINITION(ObjectBlock);

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 normal = vec4(inNormal, 0.0) * Model;

	vsOut.normal = normal.xyz;

	gl_Position = (vec4(inPosition, 1.0) * Model) * ViewProjection;
}

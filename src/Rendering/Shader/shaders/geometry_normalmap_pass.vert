#version 330 core

#include "../Interface.h"

#include "geometry_normalmap_pass.h"

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;
layout (location = TEXCOORD0)	in vec2 inTexCoord;
layout (location = TANGENT)		in vec3 inTangent;

layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlock);
layout (std140) uniform OBJECT_BLOCK_DEFINITION(ObjectBlock);

uniform sampler2D normalMap;

out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.texCoord = inTexCoord;

	vsOut.normal = normalize((vec4(inNormal, 0.0) * Model).xyz);
	vsOut.tangent = normalize((vec4(inTangent, 0.0) * Model).xyz);
	vsOut.bitangent = cross(vsOut.normal, vsOut.tangent);

	gl_Position = (vec4(inPosition, 1.0) * Model) * ViewProjection;
}

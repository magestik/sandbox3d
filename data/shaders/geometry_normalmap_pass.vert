#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

struct VS_OUTPUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};

uniform sampler2D normalMap;

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;
layout (location = TEXCOORD0)	in vec2 inTexCoord;
layout (location = TANGENT)		in vec3 inTangent;

out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.texCoord = inTexCoord;

	vsOut.normal = normalize((vec4(inNormal, 0.0) * Model).xyz);
	vsOut.tangent = normalize((vec4(inTangent, 0.0) * Model).xyz);
	vsOut.bitangent = cross(vsOut.normal, vsOut.tangent);

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

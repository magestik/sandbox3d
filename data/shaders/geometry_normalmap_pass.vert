#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

uniform sampler2D normalMap;

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;
layout (location = TEXCOORD0)	in vec2 inTexCoord;
layout (location = TANGENT)		in vec3 inTangent;

out vec2 outTexCoord;

out vec3 outNormal;
out vec3 outTangent;
out vec3 outBiTangent;

void main(void)
{
	outTexCoord = inTexCoord;

	outNormal = (vec4(inNormal, 0.0) * Model).xyz;
	outTangent = (vec4(inTangent, 0.0) * Model).xyz;
	outBiTangent = cross(outTangent, outNormal);

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

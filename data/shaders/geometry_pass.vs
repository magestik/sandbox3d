#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out vec3 outNormal;

void main(void)
{
	vec4 normal = vec4(inNormal, 0.0) * Model;

	outNormal = normal.xyz;

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

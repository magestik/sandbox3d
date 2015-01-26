
#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

struct VS_OUTPUT
{
	vec3 normal;
};

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inNormal;

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 normal = vec4(inNormal, 0.0) * Model;

	vsOut.normal = normal.xyz;

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

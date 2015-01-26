
#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

struct VS_OUTPUT
{
	vec3 position;
	vec2 texCoord;
	vec4 lightCoord;
};

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 worldPos = vec4(inPosition, 1.0) * Model;

	vsOut.position = worldPos.xyz;
	vsOut.texCoord = inTexCoord;

	vec4 pos = vec4(inPosition, 1.0) * ModelViewProjection;
	vsOut.lightCoord = pos;
	gl_Position = pos;
}

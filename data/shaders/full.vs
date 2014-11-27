#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out vec3 outPosition;
out vec2 outTexCoord;

out vec4 outLightCoord;

void main(void)
{
	vec4 worldPos = vec4(inPosition, 1.0) * Model;

	outPosition = worldPos.xyz;
	outTexCoord = inTexCoord;

	vec4 pos = vec4(inPosition, 1.0) * ModelViewProjection;
	outLightCoord = pos;
	gl_Position = pos;
}

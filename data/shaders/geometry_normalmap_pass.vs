#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2

uniform sampler2D normalMap;

uniform mat4x4 ModelViewProjection;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inTangentSpace;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out vec2 outTexCoord;
out vec4 outTangentSpace;

void main(void)
{
	outTexCoord = inTexCoord;

	outTangentSpace = vec4(inTangentSpace, sqrt(1 - inTangentSpace.x*inTangentSpace.x - inTangentSpace.y*inTangentSpace.y - inTangentSpace.z*inTangentSpace.z));

	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

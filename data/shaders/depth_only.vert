#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2

uniform mat4x4 ModelViewProjection;

layout (location = POSITION) in vec3 pos;

void main(void)
{
	gl_Position = vec4(pos, 1.0) * ModelViewProjection;
}

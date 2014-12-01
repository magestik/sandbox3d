#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2

layout (location = POSITION) in vec2 pos;
layout (location = TEXCOORD0) in vec2 tex;

out vec2 texCoord;

void main(void)
{
	texCoord = tex;
	gl_Position = vec4(pos, 0.0, 1.0);
}

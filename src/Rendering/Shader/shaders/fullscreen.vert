#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

layout (location = POSITION)	in vec2 pos;
layout (location = TEXCOORD0)	in vec2 tex;

out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.texCoord = tex;
	gl_Position = vec4(pos, 0.0, 1.0);
}

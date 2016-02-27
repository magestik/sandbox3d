#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

layout (location = POSITION) in vec2 pos;

void main(void)
{
	gl_Position = vec4(pos, 0.0, 1.0);
}

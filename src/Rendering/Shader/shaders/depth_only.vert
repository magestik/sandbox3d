#version 330 core

#include "../Interface.h"

layout (location = POSITION) in vec3 pos;

uniform mat4x4 LightViewProjection;

uniform mat4x4 Model;

void main(void)
{
	gl_Position = vec4(pos, 1.0) * Model * LightViewProjection;
}

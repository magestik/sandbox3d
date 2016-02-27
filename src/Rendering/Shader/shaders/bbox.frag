#version 330 core

#include "../Interface.h"

uniform vec3 color;

out vec3 outColor;

void main()
{
	outColor = color;
}

#version 330 core

#include "../Interface.h"

uniform vec3 ambientColor;

layout (location = 0) out vec3 outDiffuse;

void main(void)
{
        outDiffuse = ambientColor;
}




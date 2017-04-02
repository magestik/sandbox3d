#version 330 core

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D aoSampler;

uniform vec3 ambientColor;

in VS_OUTPUT vsOut;

layout (location = 0) out vec3 outDiffuse;

void main(void)
{
	float ao = texture(aoSampler, vsOut.texCoord).r;

	outDiffuse = ambientColor * ao;
}




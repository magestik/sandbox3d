#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	outColor = texture(texSampler, vsOut.texCoord).rrr;
}



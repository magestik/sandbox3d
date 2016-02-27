#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	outColor = texture(texSampler, vsOut.texCoord).rgb * 0.415; // center pixel

	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, -3)).rgb * 0.003;
	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, -2)).rgb * 0.048;
	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, -1)).rgb * 0.262;

	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, +1)).rgb * 0.262;
	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, +2)).rgb * 0.048;
	outColor += textureOffset(texSampler, vsOut.texCoord, ivec2(0, +3)).rgb * 0.003;
}



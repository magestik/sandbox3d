#version 330 core

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	vec3 sum = vec3(0.0, 0.0, 0.0);

	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(-1, -1)).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(-1,  0)).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(-1, +1)).rgb;

	sum += textureOffset(texSampler, vsOut.texCoord, ivec2( 0, -1)).rgb;
	sum += texture(texSampler, vsOut.texCoord).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2( 0, +1)).rgb;

	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(+1, -1)).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(+1,  0)).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(+1, +1)).rgb;

	outColor = sum * (1.0/9.0);
}



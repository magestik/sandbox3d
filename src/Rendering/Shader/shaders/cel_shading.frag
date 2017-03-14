#version 330 core

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D edgeSampler;
uniform sampler2D colorSampler; // a = luminance
uniform float threshold = 0.01;

in VS_OUTPUT vsOut;

out vec3 outColor;


//
// TODO : use a ramp texture instead
float ramp(float value)
{
	float intensity = 1.0;

	if (value < 0.33)
	{
		intensity = 0.33;
	}
	else if (value < 0.66)
	{
		intensity = 0.66;
	}

	return(intensity);
}

void main(void)
{
	float edge = texture(edgeSampler, vsOut.texCoord).r;
	vec4 color = texture(colorSampler, vsOut.texCoord);

	float mask = (abs(edge) < threshold) ? 1.0f : 0.0f;
	float intensity = ramp(color.a);

	outColor = color.rgb * intensity * mask;
}



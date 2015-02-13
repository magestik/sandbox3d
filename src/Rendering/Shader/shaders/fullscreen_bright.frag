#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

const vec3 LUMINANCE = vec3(0.2125f, 0.7154f, 0.0721f);

const float threshold = 0.8;

void main(void)
{
	vec3 color = texture(texSampler, vsOut.texCoord).rgb;

	float lum = dot(color, LUMINANCE);

	if (lum > threshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0, 0, 0);
	}
}

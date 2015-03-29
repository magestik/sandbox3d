#pragma GLSL(version) 330

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

const float threshold = 0.8;

void main(void)
{
	vec3 color = texture(texSampler, vsOut.texCoord).rgb;

	float lum = color.y;

	if (lum > threshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0, 0, 0);
	}
}

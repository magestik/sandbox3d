#pragma GLSL(version) 330

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec2 outLuminance;

const float DELTA = 0.0001f;

void main(void)
{
	float lum = texture(texSampler, vsOut.texCoord).y;
	outLuminance.r = log(DELTA + lum);
	outLuminance.g = lum;
}

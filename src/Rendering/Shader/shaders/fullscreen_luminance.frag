#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec2 outLuminance;

const vec3 LUMINANCE = vec3(0.2125f, 0.7154f, 0.0721f);
const float DELTA = 0.0001f;

void main(void)
{
	float lum = dot(texture(texSampler, vsOut.texCoord).rgb, LUMINANCE);
	outLuminance.r = log(DELTA + lum);
	outLuminance.g = lum;
}

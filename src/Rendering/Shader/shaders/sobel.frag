#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out float outColor;

void main(void)
{
	float px1 = textureOffset(texSampler, vsOut.texCoord, ivec2(-1, +1)).r;
	float px2 = textureOffset(texSampler, vsOut.texCoord, ivec2( 0, +1)).r;
	float px3 = textureOffset(texSampler, vsOut.texCoord, ivec2(+1, +1)).r;

	float px4 = textureOffset(texSampler, vsOut.texCoord, ivec2(-1,  0)).r;
	float px5 = texture(texSampler, vsOut.texCoord).r;
	float px6 = textureOffset(texSampler, vsOut.texCoord, ivec2(+1,  0)).r;

	float px7 = textureOffset(texSampler, vsOut.texCoord, ivec2(-1, -1)).r;
	float px8 = textureOffset(texSampler, vsOut.texCoord, ivec2( 0, -1)).r;
	float px9 = textureOffset(texSampler, vsOut.texCoord, ivec2(+1, -1)).r;

	float Gx = ((-1.0 * px1) + /*(0.0 * px2) + */ (1.0 * px3) + (-2.0 * px4) + /*(0.0 * px5) + */ (2.0 * px6) + (-1.0 * px7) + /*(0.0 * px8) + */ (1.0 * px9));
	float Gy = ((-1.0 * px1) + (-2.0 * px2) + (-1.0 * px3) + /*(0.0 * px4) + (0.0 * px5) + (0.0 * px6) +*/ (1.0 * px7) + (2.0 * px8) + (1.0 * px9));

	outColor = sqrt((Gx*Gx)+(Gy*Gy));
}



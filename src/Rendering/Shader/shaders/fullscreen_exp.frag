#pragma GLSL(version) 330

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	outColor = exp(texture(texSampler, vsOut.texCoord).rrr);
	//outColor = texture(texSampler, vsOut.texCoord).ggg;
}



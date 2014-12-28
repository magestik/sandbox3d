#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	outColor = exp(texture(texSampler, vsOut.texCoord).rrr);
}



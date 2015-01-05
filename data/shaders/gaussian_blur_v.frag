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
	vec3 sum = vec3(0.0, 0.0, 0.0);

	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(0, -1)).rgb;
	sum += texture(texSampler, vsOut.texCoord).rgb;
	sum += textureOffset(texSampler, vsOut.texCoord, ivec2(0, +1)).rgb;

	outColor = sum * (1.0/3.0);
}



#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out float outLuminance;

void main(void)
{
    outLuminance = texture(texSampler, vsOut.texCoord).r
		 + textureOffset(texSampler, vsOut.texCoord, ivec2(0, 1)).r
		 + textureOffset(texSampler, vsOut.texCoord, ivec2(1, 0)).r
		 + textureOffset(texSampler, vsOut.texCoord, ivec2(1, 1)).r; // maybe I can use linear filtering instead
}

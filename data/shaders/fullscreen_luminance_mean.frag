#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;
uniform vec2 texelSize;

in VS_OUTPUT vsOut;

out float outLuminance;

void main(void)
{
    vec2 texelSize = 1.0 / textureSize(texSampler, 0);
    outLuminance = texture(texSampler, vsOut.texCoord + vec2(0.5, 0.5)*texelSize).r;
}

#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out float outLuminance;

const vec3 LUMINANCE = vec3(0.2125f, 0.7154f, 0.0721f);
const float DELTA = 0.0001f;

void main(void)
{
    float lum = dot(texture(texSampler, vsOut.texCoord).rgb, LUMINANCE);
    outLuminance = log(DELTA + lum);
}

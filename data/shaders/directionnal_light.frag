#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D normalSampler;

uniform vec3 lightDir;
uniform vec3 lightColor;

in VS_OUTPUT vsOut;

out vec3 outColor;

void main(void)
{
	float att = 1.0;
	vec3 normal = texture(normalSampler, vsOut.texCoord).rgb * 2.0 - 1.0;
	float NdotL = clamp(dot(normal, -normalize(lightDir)), 0.0, 1.0);
	outColor = lightColor *  NdotL * att;
}




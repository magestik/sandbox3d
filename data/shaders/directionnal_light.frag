#version 330

uniform sampler2D normalSampler;

uniform vec3 lightDir;
uniform vec3 lightColor;

in vec2 texCoord;

out vec3 outColor;

void main(void)
{
	float att = 1.0;
	vec3 normal = texture(normalSampler, texCoord).rgb * 2.0 - 1.0;
	float NdotL = clamp(dot(normal, -normalize(lightDir)), 0.0, 1.0);
	outColor = lightColor *  NdotL * att;
}




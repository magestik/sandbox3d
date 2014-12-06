#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D depthSampler;
uniform sampler2D normalSampler;

uniform mat4x4 View;
uniform mat4x4 ViewProjection;

uniform vec3 lightDir;
uniform vec3 lightColor;

in VS_OUTPUT vsOut;

layout (location = 0) out vec3 outDiffuse;
layout (location = 1) out vec3 outSpecular;

vec3 reconstruct_world_pos()
{
	vec4 screenSpacePosition = vec4(vsOut.texCoord * 2.0 - 1.0, texture(depthSampler, vsOut.texCoord).r * 2.0 - 1.0, 1);
	vec4 worldSpacePosition = screenSpacePosition * inverse(ViewProjection);
	return(worldSpacePosition.xyz / worldSpacePosition.w);
}

void main(void)
{
	float att = 1.0;
	vec3 normal = texture(normalSampler, vsOut.texCoord).rgb;

	vec3 viewPos = (vec4(0.0, 0.0, 0.0, 1.0) * inverse(View)).rgb;
	vec3 viewDir = normalize(viewPos - reconstruct_world_pos());

	vec3 halfVector = normalize(lightDir + viewDir);

	float NdotL = clamp(dot(normal, lightDir), 0.0, 1.0);
	float NdotH = clamp(dot(normal, halfVector), 0.0, 1.0);

	outDiffuse = lightColor *  NdotL * att;
	outSpecular = lightColor *  pow(NdotH, 100.0) * att;
}




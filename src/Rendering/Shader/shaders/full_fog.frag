#pragma GLSL(version) 330

#include "../Interface.h"

#define USE_PCF 1

struct VS_OUTPUT
{
	vec3 position;
	vec2 texCoord;
	vec4 lightCoord;
	float depth_inView;
};

uniform mat4x4 DepthTransformation;

uniform sampler2D diffuseLightSampler;
uniform sampler2D specularLightSampler;

uniform sampler2DShadow shadowMap;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

uniform vec3 FogColor;
uniform float FogScattering;
uniform float FogExtinction;

in VS_OUTPUT vsOut;

out vec3 outColor;

uniform vec3 ambientColor;

float getShadow(vec3 position)
{
	vec4 shadowCoord = vec4(position, 1.0) * DepthTransformation;
	shadowCoord.xyz = (shadowCoord.xyz/shadowCoord.w) * vec3(0.5, 0.5, 0.5) + vec3(0.5, 0.5, 0.5);

	#if USE_PCF
		float factor = 0.0;

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(-1, 1));

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(0, 1));

		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, -1));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, 0));
		factor += textureOffset(shadowMap, shadowCoord.xyz, ivec2(1, 1));

		return (0.5 + (factor / 18.0));
	#else
		return (0.5 + texture(shadowMap, shadowCoord.xyz) * 0.5);
	#endif
}

vec3 getSpecularLight(vec4 position)
{
	vec2 fragcoord = position.xy/position.w * 0.5 + 0.5;
	return texture(specularLightSampler, fragcoord).rgb;
}

vec3 getDiffuseLight(vec4 position)
{
	vec2 fragcoord = position.xy/position.w * 0.5 + 0.5;
	return texture(diffuseLightSampler, fragcoord).rgb;
}

float getFog()
{
	float depth = vsOut.depth_inView;

	float factor = clamp(exp(- depth * FogExtinction), 0.0, 1.0);

	return(factor);
}

void main(void)
{
	vec3 diffuse = texture(diffuseSampler, vsOut.texCoord).rgb;
	vec3 specular = texture(specularSampler, vsOut.texCoord).rgb;

	float shadow_factor = getShadow(vsOut.position);
	float fog_factor = getFog();

	vec3 c = (diffuse * getDiffuseLight(vsOut.lightCoord) + specular * getSpecularLight(vsOut.lightCoord)) + ambientColor;
	outColor = (c * shadow_factor) * fog_factor + (1.0 - fog_factor) * FogColor;
}


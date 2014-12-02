#version 330

#define USE_PCF 1

struct VS_OUTPUT
{
	vec3 position;
	vec2 texCoord;
	vec4 lightCoord;
};

uniform mat4x4 DepthTransformation;

uniform sampler2D lightSampler;

uniform sampler2DShadow shadowMap;

uniform sampler2D diffuseSampler;

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

vec3 getLight(vec4 position)
{
	vec2 fragcoord = position.xy/position.w * 0.5 + 0.5;
	return texture(lightSampler, fragcoord).rgb;
}

void main(void)
{
	vec3 diffuse = texture(diffuseSampler, vsOut.texCoord).rgb;

	float shadow_factor = getShadow(vsOut.position);
	vec3 light_factor = getLight(vsOut.lightCoord);

	outColor = diffuse * shadow_factor * light_factor + ambientColor;
}


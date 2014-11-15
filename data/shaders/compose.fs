#version 330

#define USE_PCF 1

uniform mat4x4 DepthTransformation;

uniform sampler2D normalSampler;
uniform sampler2D diffuseSampler;
uniform sampler2D positionSampler;

uniform sampler2DShadow shadowMap;

uniform vec3 lightPos;

in vec2 texCoord;

out vec3 outColor;

vec3 ambient = vec3(0.3, 0.0, 0.0);

vec3 calcDirectionnalLight(vec3 WorldPos, vec3 WorldNormal)
{
	vec3 lightDiffuseColor = vec3(1.0, 1.0, 1.0);
	float lightDiffusePower = 1.0;

	vec3 lightDir = normalize(lightPos - WorldPos);

	//Intensity of the diffuse light. Saturate to keep within the 0-1 range.
	float NdotL = dot(WorldNormal, lightDir);
	float intensity = clamp(NdotL, 0.0, 1.0);

	// Calculate the diffuse light factoring in light color, power and the attenuation
	return(intensity * lightDiffuseColor * lightDiffusePower);
}

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

void main(void)
{
	vec3 color		= texture(diffuseSampler, texCoord).rgb;
	vec3 position	= texture(positionSampler, texCoord).rgb;
	vec3 normal		= texture(normalSampler, texCoord).rgb * 2.0 - 1.0;

	vec3 diffuse = color * calcDirectionnalLight(position, normal);
	float shadow_factor = getShadow(position);

	outColor =  diffuse * shadow_factor + ambient;
}


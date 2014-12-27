#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D depthSampler;
uniform sampler2D normalSampler;

uniform mat4x4 InverseViewProjection;

uniform vec3 viewPos;
uniform vec3 lightDir;
uniform vec3 lightColor;

in VS_OUTPUT vsOut;

layout (location = 0) out vec3 outDiffuse;
layout (location = 1) out vec3 outSpecular;

vec3 reconstruct_world_pos()
{
	vec4 screenSpacePosition = vec4(vsOut.texCoord * 2.0 - 1.0, texture(depthSampler, vsOut.texCoord).r * 2.0 - 1.0, 1);
	vec4 worldSpacePosition = screenSpacePosition * InverseViewProjection;
	return(worldSpacePosition.xyz / worldSpacePosition.w);
}

void main(void)
{
	float att = 1.0;
	vec4 g_buffer = texture(normalSampler, vsOut.texCoord);

	vec3 viewDir = normalize(viewPos - reconstruct_world_pos());

	vec3 halfVector = normalize(lightDir + viewDir);

	float NdotL = clamp(dot(g_buffer.rgb, lightDir), 0.0, 1.0);
	float NdotH = clamp(dot(g_buffer.rgb, halfVector), 0.0, 1.0);

        outDiffuse = lightColor * NdotL * att;
        outSpecular = lightColor * pow(NdotH, g_buffer.a) * att;
}




#version 330 core

#include "../Interface.h"

#include "fullscreen_viewray.h"

#define KERNEL_SIZE 64

uniform sampler2D depthSampler;
uniform sampler2D normalSampler;

uniform sampler2D noiseSampler;

uniform vec3 samples[KERNEL_SIZE];

uniform vec2 noiseScale;

uniform float radius = 0.5;

//layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlockFrag);
layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlock);

in VS_OUTPUT vsOut;

layout (location = 0) out float outOcclusion;

vec3 reconstruct_view_pos(vec2 screenCoord)
{
	float depth = texture(depthSampler, screenCoord).r * 2.0 - 1.0;
	vec4 screenSpacePosition = vec4(vsOut.texCoord * 2.0 - 1.0, depth, 1.0);
	vec4 viewSpacePosition = screenSpacePosition * InverseProjection;
	return(viewSpacePosition.xyz / viewSpacePosition.w);
}

float CalcViewZ(vec2 screenCoord)
{
    float depth = texture(depthSampler, screenCoord).r * 2.0 - 1.0;
    float ViewZ = Projection[3][2] / (depth - Projection[2][2]);
    return(ViewZ);
}

vec3 reconstruct_view_pos_optim(vec2 screenCoord)
{
	float viewZ = CalcViewZ(screenCoord);
	vec3 viewSpacePosition = vec3(vsOut.viewRay.x * viewZ, vsOut.viewRay.y * viewZ, viewZ);
	return(viewSpacePosition);
}

void main(void)
{
	vec3 fragPos   = reconstruct_view_pos(vsOut.texCoord);
	vec3 normal    = normalize((vec4(texture(normalSampler, vsOut.texCoord).rgb, 0.0) * View).rgb);
	vec3 randomVec = texture(noiseSampler, vsOut.texCoord * noiseScale).xyz;

	// Orthogonal basis
	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	for(int i = 0; i < KERNEL_SIZE; ++i)
	{
		// get sample position
		vec3 s = TBN * samples[i]; // From tangent to view-space
		s = fragPos + s * radius;

		vec4 offset = vec4(s, 1.0);
		offset      = offset * Projection;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		float sampleDepth = reconstruct_view_pos(offset.xy).z;

		float rangeCheck = abs(fragPos.z - sampleDepth) < radius ? 1.0 : 0.0;
		occlusion += (sampleDepth >= s.z ? 1.0 : 0.0) * rangeCheck;
	}

	outOcclusion = 1.0 - (occlusion / KERNEL_SIZE);
}

#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D depthMapSampler;

uniform float camera_near;
uniform float camera_far;

uniform vec3 FogColor;
uniform float FogScattering;
uniform float FogExtinction;

in VS_OUTPUT vsOut;

out vec4 outColor;

float getDepthInViewSpace()
{
	float depth = texture(depthMapSampler, vsOut.texCoord).r;

	return(camera_near / (camera_far - depth * (camera_far - camera_near))) * camera_far;
}

void main(void)
{
	float depth = getDepthInViewSpace();

	float factor = clamp(exp(- depth * FogExtinction), 0.0, 1.0);

	vec3 scattering = vec3(0.0f); // TODO : implement Chen 2010

	outColor = vec4(scattering, factor);
}



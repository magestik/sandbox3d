#version 330 core

#include "../Interface.h"

#include "full.h"

#define USE_PCF 1

#include "full_frag.h"

uniform vec3 diffuseColor;
uniform vec3 specularColor;

void main(void)
{
        vec3 diffuse = sRGB_to_XYZ * diffuseColor;
	vec3 specular = sRGB_to_XYZ * specularColor;

	float shadow_factor = getShadow(vsOut.position);

	outColor = shadow_factor * (diffuse * getDiffuseLight(vsOut.lightCoord) + specular * getSpecularLight(vsOut.lightCoord)) + ambientColor;
}


#version 330 core

#include "../Interface.h"

#include "geometry_pass.h"

uniform float shininess;

in VS_OUTPUT vsOut;

out vec4 fbNormal;

void main(void)
{
	fbNormal.rgb = normalize(vsOut.normal);
	fbNormal.a = shininess;
}


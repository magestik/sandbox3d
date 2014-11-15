#version 330

in vec3 outNormal;
in vec3 outPosition;
in vec2 outTexCoord;

layout (location = 0) out vec3 fbPosition;
layout (location = 1) out vec3 fbDiffuse;
layout (location = 2) out vec3 fbNormal;

void main(void)
{
	fbPosition	= outPosition;
	fbDiffuse	= vec3(1,1,1); // outTexCoord
	fbNormal	= normalize(outNormal) * 0.5 + 0.5;
}


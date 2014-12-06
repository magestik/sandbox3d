#version 330

struct VS_OUTPUT
{
	vec3 normal;
};

uniform float shinines;

in VS_OUTPUT vsOut;

out vec4 fbNormal;

void main(void)
{
	fbNormal.rgb = normalize(vsOut.normal);
	fbNormal.a = shinines;;
}


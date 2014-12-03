#version 330

struct VS_OUTPUT
{
	vec3 normal;
};

in VS_OUTPUT vsOut;

out vec3 fbNormal;

void main(void)
{
	fbNormal = normalize(vsOut.normal); // * 0.5 + 0.5;
}


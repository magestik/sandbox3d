#version 330

in vec3 outNormal;

out vec3 fbNormal;

void main(void)
{
	fbNormal = normalize(outNormal) * 0.5 + 0.5;
}


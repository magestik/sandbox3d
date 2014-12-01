#version 330

uniform sampler2D texSampler;

in vec2 texCoord;

out vec3 outColor;

void main(void)
{
	outColor = vec3(texture(texSampler, texCoord).r, 0, 0);
}



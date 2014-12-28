#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

const vec3 LUMINANCE = vec3(0.2125f, 0.7154f, 0.0721f);

void main(void)
{
    vec3 color = texture(texSampler, vsOut.texCoord).rgb;

    float lum = dot(color, LUMINANCE);

    if (lum > 0.8)
    {
	outColor = color;
    }
    else
    {
	outColor = vec3(0,0,0);
    }
}

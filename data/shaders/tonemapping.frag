#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;
uniform float avLum;

in VS_OUTPUT vsOut;

out vec3 outColor;

const float keyValue = 0.18;

const mat3x3 sRGB_to_XYZ = mat3x3(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);
const mat3x3 XYZ_to_sRGB = mat3x3(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

void main(void)
{
    vec3 color_RGB = texture(texSampler, vsOut.texCoord).rgb;
    vec3 color_XYZ = sRGB_to_XYZ * color_RGB;

    float L = (keyValue / avLum) * color_XYZ.y;
    color_XYZ.y = L / (1 + L);

    outColor = XYZ_to_sRGB * color_XYZ;
}



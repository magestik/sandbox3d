#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;
uniform float avLum;

in VS_OUTPUT vsOut;

out vec4 outColor;

const float keyValue = 0.18;

#define TONEMAP_WITH_BURNOUT 1

#if TONEMAP_WITH_BURNOUT
uniform float white2;
#endif

const mat3x3 XYZ_to_sRGB = mat3x3(3.2404542, -0.9692660, 0.0556434, -1.5371385, 1.8760108, -0.2040259, -0.4985314, 0.0415560, 1.0572252);

void main(void)
{
	vec3 color_XYZ = texture(texSampler, vsOut.texCoord).rgb;

	// XYZ -> Yxy
	vec3 color_Yxy = vec3(color_XYZ.y, color_XYZ.xy / dot(vec3(1.0,1.0,1.0), color_XYZ.xyz));

	// Tone Mapping
	float L = (keyValue / avLum) * color_Yxy.r;

	color_Yxy.r = L / (1 + L);

#if TONEMAP_WITH_BURNOUT
	color_Yxy.r *= 1 + (L / white2);
#endif

	// Yxy -> XYZ conversion

	// X = Y * x / y
	color_XYZ.r = color_Yxy.r * color_Yxy.g / color_Yxy.b;

	// copy luminance Y
	color_XYZ.g = color_Yxy.r;

	// Z = Y * z / y
	color_XYZ.b = color_Yxy.r * (1 - color_Yxy.g - color_Yxy.b) / color_Yxy.b;

	// RGB color
	outColor.rgb = sqrt(XYZ_to_sRGB * color_XYZ); // gamma 2.0
	outColor.a = sqrt(color_XYZ.g);
}



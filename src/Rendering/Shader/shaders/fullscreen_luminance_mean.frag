#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

uniform sampler2D texSampler;

out vec2 outLuminance;

void main(void)
{
	vec2 p1 = texelFetch(texSampler, ivec2(int(gl_FragCoord.x)*2,   int(gl_FragCoord.y)*2),   0).rg;
	vec2 p2 = texelFetch(texSampler, ivec2(int(gl_FragCoord.x)*2,   int(gl_FragCoord.y)*2+1), 0).rg;
	vec2 p3 = texelFetch(texSampler, ivec2(int(gl_FragCoord.x)*2+1, int(gl_FragCoord.y)*2),   0).rg;
	vec2 p4 = texelFetch(texSampler, ivec2(int(gl_FragCoord.x)*2+1, int(gl_FragCoord.y)*2+1), 0).rg;

	outLuminance.r = (p1.r + p2.r + p3.r + p4.r) * 0.25;
	outLuminance.g = max(max(p1.g, p2.g), max(p3.g, p4.g));
}

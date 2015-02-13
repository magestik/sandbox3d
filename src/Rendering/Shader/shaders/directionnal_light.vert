#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	vec2 texCoord;
};

layout (location = POSITION) in vec2 pos;
layout (location = TEXCOORD0) in vec2 tex;

out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.texCoord = tex;
	gl_Position = vec4(pos, 0.0, 1.0);
}

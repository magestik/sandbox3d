#version 330 core

#include "../Interface.h"

#include "fullscreen_viewray.h"

layout (location = POSITION)	in vec2 pos;
layout (location = TEXCOORD0)	in vec2 tex;

layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlock);

out VS_OUTPUT vsOut;

void main(void)
{
	vsOut.texCoord = tex;
	vsOut.viewRay.x = pos.x * (aspectRatio * tanHalfFOV);
    vsOut.viewRay.y = pos.y * tanHalfFOV;
	gl_Position = vec4(pos, 0.0, 1.0);
}

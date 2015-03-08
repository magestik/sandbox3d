#pragma GLSL(version) 330

#include "../Interface.h"

#include "full.h"

layout (location = POSITION)	in vec3 inPosition;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

layout (std140) uniform CAMERA_BLOCK_DEFINITION(CameraBlock);
layout (std140) uniform OBJECT_BLOCK_DEFINITION(ObjectBlock);

out VS_OUTPUT vsOut;

void main(void)
{
	vec4 worldPos = vec4(inPosition, 1.0) * Model;

	vsOut.position = worldPos.xyz;
	vsOut.texCoord = inTexCoord;

	vec4 pos = worldPos * ViewProjection;
	vsOut.lightCoord = pos;
	gl_Position = pos;
}

#include "../Interface.h"

uniform mat4x4 ModelViewProjection;

layout (location = POSITION) in vec3 pos;

void main(void)
{
	gl_Position = vec4(pos, 1.0) * ModelViewProjection;
}

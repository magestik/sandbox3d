#include "../Interface.h"

layout (location = 0) out float depth;

void main(void)
{
	depth = gl_FragCoord.z;
}


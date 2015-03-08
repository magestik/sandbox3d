#pragma GLSL(version) 330

#include "../Interface.h"

#include "pickbuffer.h"

flat in VS_OUTPUT vsOut;

out uint id;

void main(void)
{
	id = vsOut.color;
}


#pragma GLSL(version) 330

#include "../Interface.h"

struct VS_OUTPUT
{
	uint color;
};

flat in VS_OUTPUT vsOut;

out uint id;

void main(void)
{
	id = vsOut.color;
}


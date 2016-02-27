#version 330 core

#include "../Interface.h"

#include "pickbuffer.h"

flat in VS_OUTPUT vsOut;

out uint id;

void main(void)
{
	id = vsOut.color;
}


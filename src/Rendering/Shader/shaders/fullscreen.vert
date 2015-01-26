
#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

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


#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4
#define COLOR		5

#define OBJECT_BLOCK_DEFINITION(name) name \
{ \
	mat4x4 Model; \
}

#define CAMERA_BLOCK_DEFINITION(name) name \
{ \
	mat4x4 ViewProjection; \
	mat4x4 View; \
}

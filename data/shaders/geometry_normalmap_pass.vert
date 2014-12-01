#version 330

#define POSITION 	0
#define NORMAL 		1
#define TEXCOORD0	2
#define TANGENT		3
#define BITANGENT	4

uniform sampler2D normalMap;

uniform mat4x4 ModelViewProjection;
uniform mat4x4 Model;

layout (location = POSITION)	in vec3 inPosition;
layout (location = NORMAL)		in vec3 inTangentSpace;
layout (location = TEXCOORD0)	in vec2 inTexCoord;

out vec2 outTexCoord;
out vec3 outTangent;
out vec3 outBiTangent;

vec3 qrot(vec4 q, vec3 v)
{
		return v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
}

void main(void)
{
	outTexCoord = inTexCoord;
/*
		vec4 transform = vec4(inTangentSpace, sqrt(1 - inTangentSpace.x*inTangentSpace.x - inTangentSpace.y*inTangentSpace.y - inTangentSpace.z*inTangentSpace.z));

		outQuaternion = transform;

		vec3 tangent    = vec3(1.0, 0.0, 0.0);
		vec3 bitangent  = vec3(0.0, 1.0, 0.0);

		outTangent      = normalize((vec4(qrot(transform, tangent), 0.0) * Model).xyz);
		outBiTangent    = normalize((vec4(qrot(transform, bitangent), 0.0) * Model).xyz);
		outNormal       = cross(outTangent, outBiTangent);
		outNormal       = normalize(qrot(transform, vec3(0.0, 0.0, 1.0)));
*/

	vec4 transform = vec4(inTangentSpace, sqrt(1 - inTangentSpace.x*inTangentSpace.x - inTangentSpace.y*inTangentSpace.y - inTangentSpace.z*inTangentSpace.z));

	outTangent      = normalize((vec4(qrot(transform, vec3(1.0, 0.0, 0.0)), 0.0) * Model).xyz);
	outBiTangent    = normalize((vec4(qrot(transform, vec3(0.0, 1.0, 0.0)), 0.0) * Model).xyz);

/*
	vec3 outNormal = (vec4(inNormal, 0.0) * Model).xyz;
	outTangent = (vec4(inTangent, 0.0) * Model).xyz;
	outBiTangent = cross(outTangent, outNormal);
*/
	gl_Position = vec4(inPosition, 1.0) * ModelViewProjection;
}

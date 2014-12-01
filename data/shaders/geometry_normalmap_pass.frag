#version 330

uniform sampler2D normalMap;

struct VS_OUTPUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};

in VS_OUTPUT vsOut;

out vec3 fbNormal;

void main(void)
{
	vec3 normal_tangentspace = normalize(texture(normalMap, vsOut.texCoord).rgb * 2.0 - 1.0);

	vec3 normal     = normalize(vsOut.normal);
	vec3 tangent    = normalize(vsOut.tangent);
	vec3 bitangent  = normalize(vsOut.bitangent);

	fbNormal = normalize(normal_tangentspace * mat3x3(tangent, bitangent, normal)) * 0.5 + 0.5;
}


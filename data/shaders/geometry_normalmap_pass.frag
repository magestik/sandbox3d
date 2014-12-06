#version 330

struct VS_OUTPUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};

uniform sampler2D normalMap;

uniform float shinines;

in VS_OUTPUT vsOut;

out vec4 fbNormal;

void main(void)
{
	vec3 normal_tangentspace = normalize(texture(normalMap, vsOut.texCoord).rgb * 2.0 - 1.0);

	vec3 normal     = normalize(vsOut.normal);
	vec3 tangent    = normalize(vsOut.tangent);
	vec3 bitangent  = normalize(vsOut.bitangent);

	fbNormal.rgb = normalize(normal_tangentspace * inverse(mat3x3(tangent, bitangent, normal)));
	fbNormal.a = shinines;
}


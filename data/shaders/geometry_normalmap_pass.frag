#version 330

uniform sampler2D normalMap;

in vec2 outTexCoord;
in vec3 outTangent;
in vec3 outBiTangent;

out vec3 fbNormal;

void main(void)
{
	vec3 normal_tangentspace = normalize(texture(normalMap, outTexCoord).rgb * 2.0 - 1.0);

	vec3 tangent    = normalize(outTangent);
	vec3 bitangent  = normalize(outBiTangent);
	vec3 normal     = cross(bitangent, tangent);

	fbNormal = normalize(normal_tangentspace * mat3x3(tangent, bitangent, normal)) * 0.5 + 0.5;
}


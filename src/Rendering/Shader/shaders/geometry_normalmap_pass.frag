#pragma GLSL(version) 330

#include "../Interface.h"

#include "geometry_normalmap_pass.h"

uniform sampler2D normalMap;

uniform float shininess;

in VS_OUTPUT vsOut;

out vec4 fbNormal;

void main(void)
{
	vec3 normal_tangentspace = normalize(texture(normalMap, vsOut.texCoord).rgb * 2.0 - 1.0);

	vec3 normal     = normalize(vsOut.normal);
	vec3 tangent    = normalize(vsOut.tangent);
	vec3 bitangent  = normalize(vsOut.bitangent);

	fbNormal.rgb = normalize(normal_tangentspace * inverse(mat3x3(tangent, bitangent, normal)));
	fbNormal.a = shininess;
}


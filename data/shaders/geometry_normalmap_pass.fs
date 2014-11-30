#version 330

uniform sampler2D normalMap;

uniform mat4x4 Model;

in vec2 outTexCoord;
in vec4 outTangentSpace;

out vec3 fbNormal;

mat3x3 MatrixFromQuaternion(vec4 q)
{
	float xSq = q.x * q.x;
	float ySq = q.y * q.y;
	float zSq = q.z * q.z;
	float wSq = q.w * q.w;
	float twoX = 2.0f * q.x;
	float twoY = 2.0f * q.y;
	float twoW = 2.0f * q.w;
	float xy = twoX * q.y;
	float xz = twoX * q.z;
	float yz = twoY * q.z;
	float wx = twoW * q.x;
	float wy = twoW * q.y;
	float wz = twoW * q.z;

	return mat3x3(wSq + xSq - ySq - zSq, xy - wz, xz + wy,
				  xy + wz, wSq - xSq + ySq - zSq, yz - wx,
				  xz - wy, yz + wx, wSq - xSq - ySq + zSq);
}


vec3 rotate_vector( vec4 quat, vec3 vec )
{
return vec + 2.0 * cross( cross( vec, quat.xyz ) + quat.w * vec, quat.xyz );
}

void main(void)
{
	//mat3x3 TangentSpaceMatrix = MatrixFromQuaternion(normalize(outTangentSpace));

	vec3 normal_tangentspace = normalize(texture(normalMap, outTexCoord).rgb);

	vec3 normal = (vec4(rotate_vector(outTangentSpace, normal_tangentspace), 0.0) * Model).xyz;
	//vec3 normal = texture(normalMap, outTexCoord).rgb * inverse(TangentSpaceMatrix);

	fbNormal = normalize(normal) * 0.5 + 0.5;
}


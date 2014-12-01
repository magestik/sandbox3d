#ifndef UTILS_INL
#define UTILS_INL

inline mat4x4 _lookAt(const vec3 & pos, const vec3 & dir, const vec3 & up)
{
	vec3 f = normalize(dir);
	vec3 s = cross(f, normalize(up));
	vec3 u = cross(normalize(s), f);

	mat4x4 mat;
	mat[0] = vec4( s, 0.0f);
	mat[1] = vec4( u, 0.0f);
	mat[2] = vec4(-f, 0.0f);
	mat[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	mat4x4 translation(1.0f);
	translation[0][3] = -pos.x;
	translation[1][3] = -pos.y;
	translation[2][3] = -pos.z;

	return(mat * translation);
}

inline mat4x4 _perspective(float fovy, float aspect, float zNear, float zFar)
{
	float f = tanf(M_PI_2 - (fovy / 2.0f)); // cotangent(fovy/2)

	mat4x4 mat;
	mat[0] = vec4(f / aspect, 0.0f, 0.0f, 0.0f);
	mat[1] = vec4(0.0f, f, 0.0f, 0.0f);
	mat[2] = vec4(0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), (2 * zFar * zNear) / (zNear - zFar));
	mat[3] = vec4(0.0f, 0.0f, -1.0f, 0.0f);

	return(mat);
}

inline mat4x4 _frustrum(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	float twiceNearVal	= (2 * nearVal);
	float rightLeftDiff = (right - left);
	float topBottomDiff = (top - bottom);
	float farNearDiff	= (farVal - nearVal);

	float A = (right + left) / rightLeftDiff;
	float B = (top + bottom) / topBottomDiff;
	float C = - (farVal + nearVal) / farNearDiff;
	float D = - (2 * farVal * nearVal) / farNearDiff;

	mat4x4 mat;
	mat[0] = vec4(twiceNearVal / rightLeftDiff, 0.0f, A, 0.0f);
	mat[1] = vec4(0.0f, twiceNearVal / topBottomDiff, B, 0.0f);
	mat[2] = vec4(0.0f, 0.0f, C, D);
	mat[3] = vec4(0.0f, 0.0f, -1.0, 0.0f);

	return(mat);
}

inline mat4x4 _ortho(float left, float right, float bottom, float top, float nearVal, float farVal)
{

	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = (farVal + nearVal) / (farVal - nearVal);

	mat4x4 mat;
	mat[0] = vec4(2.0f / (right - left), 0.0f, 0.0f, tx);
	mat[1] = vec4(0.0f, 2.0f / (top - bottom), 0.0f, ty);
	mat[2] = vec4(0.0f, 0.0f, -2.0f / (farVal - nearVal), tz);
	mat[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return(mat);
}

#endif // UTILS_INL

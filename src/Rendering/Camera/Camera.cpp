#include "Camera.h"

#include "../utils.inl"

/**
 * @brief Camera::Camera
 */
Camera::Camera()
: m_matView(1.0f)
, m_rotation(1.0f)
, m_vPos(0.0f, 0.0f, 10.0f)
, m_vCenter(0.0f, 0.0f, 0.0f)
, m_vUp(0.0f, -1.0f, 0.0f)
{
	LookAt(m_vPos, m_vCenter, m_vUp);
}

/**
 * @brief Camera::LookAt
 * @param eye
 * @param center
 * @param up
 */
void Camera::LookAt(const vec3 & eye, const vec3 & center, const vec3 & up)
{
	vec3 dir = center - eye;
	m_matView = _lookAt(eye, dir, up);
}

/**
 * @brief Camera::Translate
 * @param x
 * @param y
 */
void Camera::TranslateXY(const vec2 & translation)
{
	m_vPos = m_vPos + vec3(translation, 0.0f);
	m_vCenter = m_vCenter + vec3(translation, 0.0f);

	LookAt(m_rotation * m_vPos, m_vCenter, m_rotation * m_vUp);
}

/**
 * @brief Camera::TranslateZ
 * @param x
 * @param y
 */
void Camera::TranslateZ(float z)
{
	m_vPos.z += z;

	LookAt(m_rotation * m_vPos, m_vCenter, m_rotation * m_vUp);
}

/**
 * @brief Camera::Rotate
 * @param x
 * @param y
 */
void Camera::Rotate(const vec2 & rotation)
{
	mat3x3 rX(1.0f);
	mat3x3 rY(1.0f);

	{
		vec3 axis_X = m_rotation * vec3(1.0f, 0, 0);

		float c = cos(rotation.y);
		float s = sin(rotation.y);

		float xs = axis_X.x * s;
		float ys = axis_X.y * s;
		float zs = axis_X.z * s;

		vec3 temp = axis_X * (1 - c);

		rX[0] = vec3((axis_X.x * temp) + vec3(c, -zs, ys));
		rX[1] = vec3((axis_X.y * temp) + vec3(zs, c, -xs));
		rX[2] = vec3((axis_X.z * temp) + vec3(-ys, xs, c));
	}

	m_rotation = rX * m_rotation;

	{
		vec3 axis_Y = m_rotation * vec3(0, 1.0f, 0);

		float c = cos(rotation.x);
		float s = sin(rotation.x);

		float xs = axis_Y.x * s;
		float ys = axis_Y.y * s;
		float zs = axis_Y.z * s;

		vec3 temp = axis_Y * (1 - c);

		rY[0] = vec3((axis_Y.x * temp) + vec3(c, -zs, ys));
		rY[1] = vec3((axis_Y.y * temp) + vec3(zs, c, -xs));
		rY[2] = vec3((axis_Y.z * temp) + vec3(-ys, xs, c));
	}

	m_rotation = rY * m_rotation;

	LookAt(m_rotation * m_vPos, m_vCenter, m_rotation * m_vUp);
}

/**
 * @brief Camera::getViewProjection
 * @return
 */
const mat4x4 & Camera::getViewMatrix(void) const
{
	return(m_matView);
}



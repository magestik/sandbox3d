#pragma once

#include <Vector.h>
#include <Matrix.h>

class Camera
{
public:
	Camera();

	void	LookAt			(const vec3 & eye, const vec3 & center, const vec3 & up);

	void	SetCenter		(const vec3 & center);
	void	SetUp			(const vec3 & up);
	void	SetPosition		(const vec3 & pos);

	void	TranslateXY		(const vec2 & translation);
	void	TranslateZ		(float translation);
	void	Rotate			(const vec2 & rotation);

	const mat4x4 &	getViewMatrix	(void) const;

private:

	mat4x4 m_matView;

	mat3x3	m_rotation;

	vec3	m_vPos;
	vec3	m_vCenter;
	vec3	m_vUp;

};

#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>

namespace tinyxml2
{
	class XMLElement;
}

class Rendering;
class Shader;

class Pass
{

public:

	Pass();
	Pass(const tinyxml2::XMLElement * element, const Rendering & rendering);

	bool	begin	(void);
	void	end		(void);

	void		SetUniform		(const char * name, const mat4x4 & m);
	void		SetUniform		(const char * name, const mat3x3 & m);
	void		SetUniform		(const char * name, const mat2x2 & m);

	void		SetUniform		(const char * name, const vec4 & v);
	void		SetUniform		(const char * name, const vec3 & v);
	void		SetUniform		(const char * name, const vec2 & v);

	void		SetUniform		(const char * name, int n);
	void		SetUniform		(const char * name, float n);

	template<GLenum D>
	void		SetTexture		(const char * name, int unit, const GPU::Texture<D> & texture);

protected:

	GLuint m_uFramebufferObject;
	GLuint m_uShaderObject;

	std::vector<GLenum> m_aDrawBuffers;
};

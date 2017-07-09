#pragma once

// Maths
#include <Vector.h>
#include <Matrix.h>

// std also needed
#include <map>
#include <vector>
#include <string>
#include <queue>

// Rendering Hardware Interface
#include "RHI/RHI.h"

// class definitions
#include "Light/Light.h"
#include "Mesh/Mesh.h"
#include "Mesh/SubMesh.h"
#include "Scene/Scene.h"
#include "Scene/SceneListener.h"

#include "RenderTexture.h"
#include "GraphicsAlgorithm.h"

#include "Environment.h"

#include "Shader/Interface.h"

// theses maps contains all shaders, compiled at launch time
extern std::map<std::string, GLuint> g_Textures;

extern std::map<std::string, Mesh>			g_Meshes;

inline void SetUniform(GLuint uShaderObject, const char * name, const mat2x2 & m)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix2fv(location, 1, false, (float*)&m);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const mat3x3 & m)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix3fv(location, 1, false, (float*)&m);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const mat4x4 & m)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniformMatrix4fv(location, 1, false, (float*)&m);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const vec4 & v)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform4fv(location, 1, (float*)&v);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const vec3 & v)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform3fv(location, 1, (float*)&v);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const vec3 * v, unsigned int count)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform3fv(location, count, (float*)v);
}

inline void SetUniform(GLuint uShaderObject, const char * name, const vec2 & v)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform2fv(location, 1, (float*)&v);
}

inline void SetUniform(GLuint uShaderObject, const char * name, int n)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform1i(location, n);
}

inline void SetUniform(GLuint uShaderObject, const char * name, unsigned int n)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform1ui(location, n);
}

inline void SetUniform(GLuint uShaderObject, const char * name, float n)
{
	GLint location = glGetUniformLocation(uShaderObject, name);
	//assert(-1 != location);
	glUniform1f(location, n);
}

inline void SetUniformBlockBinding(GLuint uShaderObject, const char * name, unsigned int binding)
{
	GLuint blockIndex = glGetUniformBlockIndex(uShaderObject, name);

	if (GL_INVALID_INDEX != blockIndex)
	{
		glUniformBlockBinding(uShaderObject, blockIndex, binding);
	}
}

template<GLenum D>
inline void SetTexture(GLuint uShaderObject, const char * name, int unit, const GPU::Texture<D> & texture, RHI::Sampler & sampler)
{
	SetUniform(uShaderObject, name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(D, texture.GetObject());
	glBindSampler(unit, sampler.m_uSamplerObject);
}

template<GLenum D>
inline void SetTexture(GLuint uShaderObject, const char * name, int unit, GLuint textureId, RHI::Sampler & sampler)
{
	SetUniform(uShaderObject, name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(D, textureId);
	glBindSampler(unit, sampler.m_uSamplerObject);
}

class Rendering : public SceneListener
{

	friend class RenderXML;

public:

	explicit Rendering		(void);

	void	onReady					(void);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor);

	virtual void	onObjectInserted	(const Scene & scene, const Object & object) override;
	virtual void	onObjectRemoved		(const Scene & scene, const Object & object) override;

	void initQueue					(const char * szFilename);
	void releaseQueue				(void);

	unsigned int GetWidth(void) const
	{
		return(m_uWidth);
	}

	unsigned int GetHeight(void) const
	{
		return(m_uHeight);
	}

	const GPU::Texture<GL_TEXTURE_2D> * GetRenderTexture(const char * name) const
	{
		std::map<std::string, RenderTexture>::const_iterator it = m_mapTargets.find(name);

		if (it != m_mapTargets.end())
		{
			return((*it).second.getTexture());
		}

		return(nullptr);
	}

	void SetDefaultFramebuffer(GLuint framebuffer)
	{
		m_defaultFramebuffer = RHI::Framebuffer(framebuffer);
	}

	void SetPickBufferFramebuffer(GLuint framebuffer)
	{
		m_pickBufferFramebuffer = RHI::Framebuffer(framebuffer);
	}

protected:

	void	initShaders					(void);

	void	generateMeshes				(void);

	void    updateCameraBuffer          (const mat4x4 & matView);
	void    updateObjectsBuffer         (void);

	void	initPickBuffer				(void);
	void	renderPickBuffer			(void);

	void	computeToneMappingParams	(float & avLum, float & white2);

//private:
public:

	struct CAMERA_BLOCK_DEFINITION(CameraBlock);
	struct OBJECT_BLOCK_DEFINITION(ObjectBlock);

	unsigned int m_uWidth;
	unsigned int m_uHeight;

	float m_fMinZ;
	float m_fMaxZ;

	mat4x4 m_matProjection;
	mat4x4 m_matShadowMapProjection;

	Scene m_scene;

	Mesh *	m_pQuadMesh;

	enum EUniformBlockBinding
	{
		BLOCK_BINDING_CAMERA = 0,
		BLOCK_BINDING_OBJECT = 1
	};

	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pCameraBuffer;
	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pObjectsBuffer;

	std::map<std::string, RenderTexture>	m_mapTargets;

	std::map<std::string, RHI::ShaderModule> m_mapShaderModules;
	std::map<std::string, RHI::Framebuffer>	m_mapFramebuffer;
	RHI::Framebuffer m_defaultFramebuffer;

	std::vector<GraphicsAlgorithm*> m_renderQueue;

	mat4x4	m_matCurrentView;
	vec4	m_vCurrentClearColor;

	//
	// Pick-Buffer
	RHI::Framebuffer m_pickBufferFramebuffer;

	RHI::Pipeline m_pickBufferPipeline;
	RHI::RenderPass m_pickBufferRenderPass;

	bool m_bReady;
	bool m_bInitialized;

public:

	EnvironmentSettings environment;

};

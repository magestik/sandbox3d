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
#include "Mesh/Mesh.h"
#include "Mesh/SubMesh.h"

#include "Scene.h"
#include "SceneListener.h"
#include "Resources/ResourceManagerListener.h"

#include "GraphicsAlgorithm.h"

#include "Environment.h"

#include "Shader/Interface.h"

#include "RenderGraph.h"

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

class Rendering : public SceneListener, public ResourceManagerListener
{

	friend class RenderXML;

public:

	explicit Rendering				(Scene & scene, RenderGraph::Factory & factory);

	void	onReady					(const char * szBaseDir);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor);

	unsigned int GetWidth(void) const
	{
		return(m_uWidth);
	}

	unsigned int GetHeight(void) const
	{
		return(m_uHeight);
	}

	GLuint GetTexture(unsigned int TextureID) const
	{
		return(m_aLoadedTextures[TextureID-1]);
	}

	Mesh * GetMesh(unsigned int MeshID) const
	{
		return(m_aLoadedMeshes[MeshID-1]);
	}

	const Scene & GetScene(void) const
	{
		return(m_scene);
	}

	Scene & GetScene(void)
	{
		return(m_scene);
	}

protected:

	void	initShaders					(const char * szShadersDir);

	void	generateMeshes				(void);

	void    updateCameraBuffer          (const mat4x4 & matView);
	void    updateObjectsBuffer         (void);

	void	computeToneMappingParams	(float & avLum, float & white2);

private: // implements SceneListener / ResourceManagerListener

	virtual void	onMeshImported		(const ResourceManager & scene, unsigned int MeshID, const VertexData & vertexData) override;
	virtual void	onMeshImported		(const ResourceManager & scene, unsigned int MeshID, const VertexData & vertexData, const IndexData & indexData) override;

	virtual void	onTextureImported	(const ResourceManager & scene, unsigned int TextureID, const TextureData1D & textureData) override;
	virtual void	onTextureImported	(const ResourceManager & scene, unsigned int TextureID, const TextureData2D & textureData) override;
	virtual void	onTextureImported	(const ResourceManager & scene, unsigned int TextureID, const TextureData3D & textureData) override;

	virtual void	onObjectInserted	(const Scene & scene, const Object & object) override;
	virtual void	onObjectRemoved		(const Scene & scene, const Object & object) override;

	virtual void	onCameraInserted	(const Scene & scene, const Camera & camera) override;
	virtual void	onCameraRemoved		(const Scene & scene, const Camera & camera) override;

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

private:

	Scene & m_scene;

	std::vector<Mesh*> m_aLoadedMeshes; // onMeshImported
	std::vector<GLuint> m_aLoadedTextures; // onTextureImported

public:

	Mesh *	m_pQuadMesh;

	enum EUniformBlockBinding
	{
		BLOCK_BINDING_CAMERA = 0,
		BLOCK_BINDING_OBJECT = 1
	};

	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pCameraBuffer;
	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pObjectsBuffer;

	std::map<std::string, RHI::ShaderModule> m_mapShaderModules;

	mat4x4	m_matCurrentView;
	vec4	m_vCurrentClearColor;

	//
	// Pick-Buffer
	RHI::Pipeline m_pickBufferPipeline;
	RHI::RenderPass m_pickBufferRenderPass;

	bool m_bReady;
	bool m_bInitialized;

	enum DEPTH_CLIP_SPACE
	{
		ZERO_TO_ONE,
		NEGATIVE_ONE_TO_ONE
	};

	DEPTH_CLIP_SPACE m_depthClipSpace;

public:

	EnvironmentSettings environment;

};

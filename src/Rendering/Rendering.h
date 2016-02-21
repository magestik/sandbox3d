#pragma once

// Maths
#include <Vector.h>
#include <Matrix.h>

// std also needed
#include <map>
#include <vector>
#include <string>

// class definitions
#include "Light/Light.h"
#include "Mesh/Mesh.h"
#include "Mesh/SubMesh.h"

#include "Target/ShadowMap.h"
#include "Target/AverageLuminance.h"

#include "Pass.h"
#include "RenderTexture.h"
#include "Pipeline.h"

#include "Environment.h"

#include "Shader/Interface.h"

// theses maps contains all shaders, compiled at launch time
extern std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *>	g_FragmentShaders;
extern std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *>	g_VertexShaders;
extern std::map<std::string, GPU::Shader<GL_GEOMETRY_SHADER> *>	g_GeometryShaders;

extern std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

extern std::map<std::string, Mesh>			g_Meshes;

namespace tinyxml2
{
	class XMLElement;
}

class Rendering
{

public:

	enum ERenderType
	{
		FINAL,
		SPECULAR_LIGHTS,
		DIFFUSE_LIGHTS,
		NORMAL_BUFFER,
		DEPTH,
		SHADOWS,
		LUMINANCE1,
		LUMINANCE2,
		BLOOM
	};

	explicit Rendering		(void);

	void	onInitializeComplete	(void);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor, bool bWireframe, ERenderType eRenderType = FINAL, const Mesh::Instance * pSelected = nullptr);

	void	onCreate				(const Mesh::Instance & instance);

	void	onDelete				(const Mesh::Instance & instance);

	Mesh::Instance * getObjectAtPos	(const ivec2 & pos);

	const GPU::Shader<GL_VERTEX_SHADER> * GetVertexShader(const char * name) const
	{
		return(g_VertexShaders[name]);
	}

	const GPU::Shader<GL_FRAGMENT_SHADER> * GetFragmentShader(const char * name) const
	{
		return(g_FragmentShaders[name]);
	}

	const GPU::Shader<GL_GEOMETRY_SHADER> * GetGeometryShader(const char * name) const
	{
		return(g_GeometryShaders[name]);
	}

	const GPU::Texture<GL_TEXTURE_2D> * GetRenderTexture(const char * name) const
	{
		return(m_mapTargets.at(name).getTexture());
	}

	const Pipeline * GetPipeline(const char * name) const
	{
		return(m_mapPipeline.at(name));
	}

protected:

	void	initializePipelineFromXML	(const char * filename);
	void	initializePipelines     	(const tinyxml2::XMLElement * pipelines);
	void	initializeTargets			(const tinyxml2::XMLElement * targets);
	void	initializePasses			(const tinyxml2::XMLElement * passes);

	void	generateMeshes				(void);

	void    updateCameraBuffer          (const mat4x4 & matView);
	void    updateObjectsBuffer         (void);

	void    computeAverageLum           (void);

	void	renderSceneToGBuffer		(void);

	void	renderLightsToAccumBuffer	(const mat4x4 & mView);

	void	renderSceneToShadowMap		(void);

	void    renderBloom                 (void);

	void	renderFinal					(const mat4x4 & mView, const vec4 & clearColor);

	void    renderFog                   (void);

	void	renderIntermediateToScreen	(ERenderType eRenderType);

	void    renderPostProcessEffects    (void);

	void	renderPickBuffer			(void);

	void	renderBoundingBox			(const Mesh::Instance * pSelected);

private:

	struct CAMERA_BLOCK_DEFINITION(CameraBlock);
	struct OBJECT_BLOCK_DEFINITION(ObjectBlock);

	unsigned int m_uWidth;
	unsigned int m_uHeight;

	unsigned int m_uLuminanceSizePOT;

	mat4x4 m_matProjection;

	AverageLuminance * m_AvLum;

	std::vector<Mesh::Instance> m_aObjects;

	ShadowMap * m_pShadowMap;
	Light::Directionnal * m_pLight;

	Mesh *	m_pQuadMesh;
	Mesh *	m_pPointMesh;

	enum EUniformBlockBinding
	{
		BLOCK_BINDING_CAMERA = 0,
		BLOCK_BINDING_OBJECT = 1
	};

	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pCameraBuffer;
	GPU::Buffer<GL_UNIFORM_BUFFER> * m_pObjectsBuffer;

	std::map<std::string, RenderTexture>	m_mapTargets;
	std::map<std::string, Pass>		m_mapTechnique;
	std::map<std::string, const Pipeline *>       m_mapPipeline;

public:

	EnvironmentSettings environment;

};

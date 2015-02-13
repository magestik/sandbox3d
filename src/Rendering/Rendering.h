#pragma once

// Maths
#include <Vector.h>
#include <Matrix.h>

// std also needed
#include <map>
#include <vector>
#include <string>

// class definitions
#include "Shader/Shader.h"
#include "Light/Light.h"
#include "Mesh/Mesh.h"
#include "Mesh/SubMesh.h"

#include "Target/ShadowMap.h"
#include "Target/LightAccumBuffer.h"
#include "Target/AverageLuminance.h"
#include "Target/Bloom.h"

#include "RenderTexture.h"

// theses maps contains all shaders, compiled at launch time
extern std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *>	g_FragmentShaders;
extern std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *>	g_VertexShaders;

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

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor, bool bWireframe, ERenderType eRenderType = FINAL);

	void	onCreate				(const Mesh::Instance & instance);

	const GPU::Shader<GL_VERTEX_SHADER> * GetVertexShader(const char * name) const
	{
		return(g_VertexShaders[name]);
	}

	const GPU::Shader<GL_FRAGMENT_SHADER> * GetFragmentShader(const char * name) const
	{
		return(g_FragmentShaders[name]);
	}

	const GPU::Texture<GL_TEXTURE_2D> * GetRenderTexture(const char * name) const
	{
		return(m_mapTargets.at(name).getTexture());
	}

protected:

	void	initializePipelineFromXML	(const char * filename);
	void	initializeTargets			(const tinyxml2::XMLElement * targets);
	void	initializePipeline			(const tinyxml2::XMLElement * pipeline);

	void	compileShaders				(void);
	void	generateMeshes				(void);

	void    computeAverageLum           (void);

	void	renderSceneToGBuffer		(const mat4x4 & mView);

	void	renderLightsToAccumBuffer	(const mat4x4 & mView);

	void	renderSceneToShadowMap		(void);

	void    renderBloom                 (void);

	void	renderFinal					(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor);

	void	renderIntermediateToScreen	(ERenderType eRenderType);

	void    renderPostProcessEffects    (void);

private:

	unsigned int m_uWidth;
	unsigned int m_uHeight;

	unsigned int m_uLuminanceSizePOT;

	mat4x4 m_matProjection;

	LightAccumBuffer	m_LightAccumBuffer;
	AverageLuminance    m_AvLum;
	Bloom               m_BloomPass;

	std::vector<Mesh::Instance> m_aObjects;

	ShadowMap * m_pShadowMap;
	Light::Directionnal * m_pLight;

	Shader *	m_pDepthOnlyPassShader;

	Shader *	m_pFullscreenDepthShader;
	Shader *	m_pFullscreenNormalShader;
	Shader *	m_pFullscreenColorShader;
	Shader *	m_pFullscreenExpShader;

	Shader *	m_pToneMappingShader;

	Shader *	m_pDirectionnalLightShader;

	SubMesh *	m_pQuadMesh;

	enum ETarget
	{
		TARGET_LUMINANCE1       = 0,
		TARGET_LUMINANCE2       = 1,
		TARGET_MAX              = 2
	};

	GPU::Texture<GL_TEXTURE_2D> * m_apTargets [TARGET_MAX];



	GLuint m_uSampler;


	std::map<std::string, RenderTexture>	m_mapTargets;
	std::map<std::string, Technique>		m_mapTechnique;
};

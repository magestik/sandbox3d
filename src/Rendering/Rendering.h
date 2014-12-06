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

#include "Target/GBuffer.h"
#include "Target/ShadowMap.h"
#include "Target/LightAccumBuffer.h"
#include "Target/Final.h"

// theses maps contains all shaders, compiled at launch time
extern std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *>	g_FragmentShaders;
extern std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *>	g_VertexShaders;

extern std::map<std::string, GPU::Texture<GL_TEXTURE_2D> *> g_Textures;

extern std::map<std::string, Mesh>			g_Meshes;

class Rendering
{

public:

	enum ERenderType
	{
		FINAL,
		LIGHT_BUFFER,
		NORMAL_BUFFER,
		DEPTH,
		SHADOWS
	};

	explicit Rendering		(void);

	void	onInitializeComplete	(void);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor, bool bWireframe, ERenderType eRenderType = FINAL);

	void	onCreate				(const Mesh::Instance & instance);

protected:

	void	compileShaders				(void);
	void	generateMeshes				(void);

	void	renderSceneToGBuffer		(const mat4x4 & mView);

	void	renderLightsToAccumBuffer	(const mat4x4 & mView);

	void	renderSceneToShadowMap		(void);

	void	renderIntermediateToScreen	(ERenderType eRenderType);

	void	renderFinal					(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor);

private:

	unsigned int m_uWidth;
	unsigned int m_uHeight;

	mat4x4 m_matProjection;

	GBuffer				m_GBuffer;
	Final				m_Compose;
	LightAccumBuffer	m_LightAccumBuffer;

	std::vector<Mesh::Instance> m_aObjects;

	ShadowMap * m_pShadowMap;
	Light::Directionnal * m_pLight;

	Shader *	m_pComposeShader;

	Shader *	m_pGeometryPassShader;
	Shader *	m_pGeometryWithNormalMapPassShader;

	Shader *	m_pDepthOnlyPassShader;

	Shader *	m_pFullscreenDepthShader;
	Shader *	m_pFullscreenNormalShader;

	Shader *	m_pDirectionnalLightShader;

	SubMesh *	m_pQuadMesh;

	enum ETarget
	{
		TARGET_DEPTH,
		TARGET_NORMALS,
		TARGET_DIFFUSE_LIGHTS,
		TARGET_SPECULAR_LIGHTS,
		TARGET_POSTFX1,
		TARGET_POSTFX2
	};

	GPU::Texture<GL_TEXTURE_2D> * m_apTargets [6];

	GLuint m_uSampler;
};

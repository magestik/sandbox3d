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
#include "Target/AverageLuminance.h"

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
		SPECULAR_LIGHTS,
		DIFFUSE_LIGHTS,
		NORMAL_BUFFER,
		DEPTH,
        SHADOWS,
        LUMINANCE1,
        LUMINANCE2
	};

	explicit Rendering		(void);

	void	onInitializeComplete	(void);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor, bool bWireframe, ERenderType eRenderType = FINAL);

	void	onCreate				(const Mesh::Instance & instance);

protected:

	void	compileShaders				(void);
	void	generateMeshes				(void);

    void    computeAverageLum           (void);

	void	renderSceneToGBuffer		(const mat4x4 & mView);

	void	renderLightsToAccumBuffer	(const mat4x4 & mView);

	void	renderSceneToShadowMap		(void);

	void	renderIntermediateToScreen	(ERenderType eRenderType);

	void	renderFinal					(const mat4x4 & mView, const vec4 & clearColor, const vec4 & ambientColor);

private:

	unsigned int m_uWidth;
	unsigned int m_uHeight;

    unsigned int m_uLuminanceSizePOT;

	mat4x4 m_matProjection;

	GBuffer				m_GBuffer;
	Final				m_Compose;
	LightAccumBuffer	m_LightAccumBuffer;
    AverageLuminance    m_AvLum;

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
        TARGET_DEPTH            = 0,
        TARGET_NORMALS          = 1,
        TARGET_DIFFUSE_LIGHTS   = 2,
        TARGET_SPECULAR_LIGHTS  = 3,
        TARGET_FINAL_HDR        = 4,
        TARGET_LUMINANCE1       = 5,
        TARGET_LUMINANCE2       = 6,
        TARGET_POSTFX1          = 7,
        TARGET_POSTFX2          = 8,
        TARGET_MAX              = 9
	};

    GPU::Texture<GL_TEXTURE_2D> * m_apTargets [TARGET_MAX];

	GLuint m_uSampler;
};

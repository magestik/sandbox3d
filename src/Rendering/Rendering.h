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
#include "Camera/Camera.h"
#include "Light/Light.h"
#include "Mesh/Mesh.h"

#include "Target/GBuffer.h"
#include "Target/ShadowMap.h"
#include "Target/LightAccumBuffer.h"

// theses maps contains all shaders, compiled at launch time
extern std::map<std::string, GPU::Shader<GL_FRAGMENT_SHADER> *>	g_FragmentShaders;
extern std::map<std::string, GPU::Shader<GL_VERTEX_SHADER> *>	g_VertexShaders;

extern std::map<std::string, Mesh*>			g_Meshes;

extern Mesh * g_pQuadMesh;

class Rendering
{

public:

	enum ERenderType
	{
		FINAL,
		POSITION,
		NORMAL,
		DIFFUSE,
		DEPTH,
		SHADOWS
	};

	explicit Rendering		(void);

	void	onInitializeComplete	(void);

	void	onResize				(int width, int height);

	void	onUpdate				(const mat4x4 & mView, bool bWireframe, ERenderType eRenderType = FINAL);

	void	onCreate				(Mesh * m);

protected:

	void	renderSceneToGBuffer		(const mat4x4 & mView);

	void	renderLightsToAccumBuffer	(void);

	void	renderSceneToShadowMap		(void);

	void	renderIntermediateToScreen	(ERenderType eRenderType);

	void	renderFinal					(void);

private:

	GBuffer	m_gBuffer;

	ShadowMap m_shadowMap;

	LightAccumBuffer m_lightAccumBuffer;

	std::vector<Mesh::Instance> m_aObjects;

	Light::Spot * m_pLight;

	Shader * m_pGeometryPassShader;
	Shader * m_pLightPassShader;
	Shader * m_pDepthOnlyPassShader;

	Shader * m_pFullscreenDepthShader;
	Shader * m_pFullscreenNormalShader;
	Shader * m_pFullscreenComposeShader;
};

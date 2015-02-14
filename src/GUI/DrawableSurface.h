#pragma once

#include "../Rendering/Rendering.h"

#include <QGLWidget>

#include "Camera/Camera.h"

#define NB_BUFFER 10

class QDir;
class QString;

class aiScene;
class aiNode;

class DrawableSurface : public QGLWidget
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);

	void	DebugFinal			(void);
	void	DebugNormal			(void);
	void	DebugDiffuseLights	(void);
	void	DebugSpecularLights	(void);
	void	DebugDepth			(void);
	void	DebugShadows		(void);
	void	DebugLuminance		(int num);
	void	DebugBloom          (void);

	void	DebugWireframe		(bool d);

protected:

	void	initializeGL		(void);
	void	resizeGL			(int w, int h);
	void	paintGL				(void);

	void	mousePressEvent		(QMouseEvent * event);
	void	mouseMoveEvent		(QMouseEvent * event);
	void	mouseReleaseEvent	(QMouseEvent * event);

	void	wheelEvent			(QWheelEvent * event);

private:

	void	loadShaders			(void);
	void	loadSprites			(void);

	GPU::Texture<GL_TEXTURE_2D> * loadTexture(const QString & filepath);

	void loadAllMaterials(const aiScene * scene);

	void addMeshRecursive(const aiNode * nd, const mat4x4 & parentTransformation, const std::vector<SubMesh*> & preloaded, const GPU::Buffer<GL_ARRAY_BUFFER> & vertexBuffer, const GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> & indexBuffer);

	// Rendering
	Camera		m_camera;

	// Navigation
	ivec2		m_vLastPos;

	// Debug
	Rendering::ERenderType	m_eRenderType;
	bool					m_bDebugWireframe;

	//
	unsigned int m_query;

	GPU::Buffer<GL_PIXEL_UNPACK_BUFFER> * m_apBuffer [NB_BUFFER];
	GPU::Buffer<GL_PIXEL_UNPACK_BUFFER> * m_pSpecial;

	vec4 m_vClearColor;
	vec4 m_vAmbientColor;

	std::vector<Mesh*> m_apMeshes;

public:

	Rendering	m_renderer;

signals:

public slots:

	void reloadShader(const QString & filename);

	void importScene(const QString & filename);

	void setClearColor(const QColor & color);
	void setAmbientColor(const QColor & color);
};

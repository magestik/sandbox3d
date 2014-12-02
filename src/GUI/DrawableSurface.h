#pragma once

#include "../Rendering/Rendering.h"

#include <QGLWidget>

#define NB_BUFFER 10

class DrawableSurface : public QGLWidget
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);
	void	AddObject			(const std::string & name);

	void	DebugFinal			(void);
	void	DebugNormal			(void);
	void	DebugLights			(void);
	void	DebugDepth			(void);
	void	DebugShadows		(void);

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
	void	loadMeshes			(void);
	void	loadSprites			(void);

	// Rendering
	Camera		m_camera;
	Rendering	m_renderer;

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

signals:

public slots:

	void reloadShader(const QString & filename);

	void setClearColor(const QColor & color);
	void setAmbientColor(const QColor & color);
};

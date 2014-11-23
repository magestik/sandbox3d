#pragma once

#include "../Rendering/Rendering.h"

#include <QGLWidget>

class DrawableSurface : public QGLWidget
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);
	void	AddObject			(const std::string & name);

	void	DebugFinal			(void);
	void	DebugDiffuse		(void);
	void	DebugNormal			(void);
	void	DebugPosition		(void);
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

signals:

public slots:

	void setClearColor(const QColor & color);
};

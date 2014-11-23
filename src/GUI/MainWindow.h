#pragma once

#include <QMainWindow>

class QGLWidget;
class QDockWidget;
class QColorDialog;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit	MainWindow		(QWidget *parent = 0);
	virtual		~MainWindow		(void);

	void		closeEvent		(QCloseEvent * event);

	void		SetRenderTime	(double cpu_time, double gpu_time);
	void		SetStatus		(const QString & str);

private:

	Ui::MainWindow * ui;

public:

	QGLWidget * m_pDrawable;
	QDockWidget * m_pMeshListDock;
	QColorDialog * m_pClearColorChooser;

private slots:

	void on_actionWireframe_toggled(bool checked);
	void on_actionClear_color_triggered();
	void on_actionFaceCulling_changed();
	void on_actionResetCamera_triggered();
	void on_actionMeshList_toggled(bool checked);

	void on_actionFinal_toggled(bool checked);
	void on_actionDiffuse_toggled(bool checked);
	void on_actionNormal_toggled(bool checked);
	void on_actionPosition_toggled(bool checked);
	void on_actionDepth_toggled(bool checked);
	void on_actionShadows_toggled(bool checked);
};

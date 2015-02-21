#pragma once

#include <QMainWindow>

#include "EnvSettingsWidget.h"

class QDockWidget;
class QFileDialog;
class QFileSystemWatcher;

class DrawableSurface;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit	MainWindow		(QWidget * parent = 0);
    virtual		~MainWindow		(void);

    void		closeEvent		(QCloseEvent * event);

    void		SetRenderTime	(double cpu_time, double gpu_time);
    void		SetStatus		(const QString & str);

private:

    Ui::MainWindow * ui;

protected:

    EnvSettingsWidget * m_pEnvSettingsWidget;

public:

    DrawableSurface * m_pDrawable;

    QColorDialog * m_pClearColorChooser;

    QFileDialog * m_pFileChooser;

    QFileSystemWatcher * m_pFileWatcher;

private slots:

    void on_actionImport_triggered();

    void on_actionClear_color_triggered();


    void on_actionWireframe_toggled(bool checked);
    void on_actionFaceCulling_changed();
    void on_actionResetCamera_triggered();

    void on_actionFinal_toggled(bool checked);
    void on_actionNormal_toggled(bool checked);
    void on_actionLightSpecular_toggled(bool checked);
    void on_actionLightDiffuse_toggled(bool checked);
    void on_actionDepth_toggled(bool checked);
    void on_actionShadows_toggled(bool checked);
    void on_actionLuminance1_toggled(bool checked);
    void on_actionLuminance2_toggled(bool checked);
    void on_actionBloom_toggled(bool checked);
};

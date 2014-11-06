#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "DrawableSurface.h"
#include "MeshListWidget.h"

#include <QSettings>
#include <QDockWidget>
#include <QColorDialog>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	QSettings settings;

	ui->setupUi(this);

	m_pDrawable = new DrawableSurface(this);
	setCentralWidget(m_pDrawable);

	{
		m_pMeshListDock = new MeshListWidget(this);
		m_pMeshListDock->hide();
		//connect(m_pMeshListDock, SIGNAL(), this, SLOT());
	}

	{
		m_pClearColorChooser = new QColorDialog(this);
		m_pClearColorChooser->hide();
		m_pClearColorChooser->setWindowTitle("Clear Color Chooser");
		connect(m_pClearColorChooser, SIGNAL(currentColorChanged(const QColor &)), m_pDrawable, SLOT(setClearColor(const QColor &)));
		m_pClearColorChooser->setCurrentColor(QColor::fromRgbF(0.5f, 0.5f, 0.5f));
	}

	//restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	//restoreState(settings.value("mainWindowState").toByteArray());

	ui->statusbar->showMessage(tr("Ready"), 2000);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent * event)
{
	QSettings settings;
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
}

/**
 * @brief MainWindow::SetRenderTime
 * @param t
 */
void MainWindow::SetRenderTime(double t)
{
	if (t < 4.0)
	{
		QString str = QString("Render Time : %1 ms").arg(t);
		ui->statusbar->showMessage(str);
		ui->statusbar->setStyleSheet("* {color : black; }");
	}
	else
	{
		QString str = QString("Render Time : %1 ms (> 4 ms)").arg(t);
		ui->statusbar->showMessage(str);
		ui->statusbar->setStyleSheet("* {color : red; }");
	}
}

/**
 * @brief MainWindow::on_actionWireframe_changed
 */
void MainWindow::on_actionWireframe_toggled(bool checked)
{
	static_cast<DrawableSurface*>(m_pDrawable)->DebugWireframe(checked);
}

/**
 * @brief MainWindow::on_actionFaceCulling_changed
 */
void MainWindow::on_actionFaceCulling_changed()
{
	if (ui->actionFaceCulling->isChecked())
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

/**
 * @brief MainWindow::on_actionClear_color_clicked
 */
void MainWindow::on_actionClear_color_triggered()
{
	m_pClearColorChooser->show();
}

/**
 * @brief MainWindow::on_actionClear_color_clicked
 */
void MainWindow::on_actionResetCamera_triggered()
{
	static_cast<DrawableSurface*>(m_pDrawable)->ResetCamera();
}

/**
 * @brief MainWindow::on_actionMeshList_toggled
 * @param checked
 */
void MainWindow::on_actionMeshList_toggled(bool checked)
{
	if (checked)
	{
		m_pMeshListDock->show();
	}
	else
	{
		m_pMeshListDock->hide();
	}
}

/**
 * @brief MainWindow::on_actionFinal_toggled
 * @param checked
 */
void MainWindow::on_actionFinal_toggled(bool checked)
{
	if (checked)
	{
		ui->actionDiffuse->setChecked(false);
		ui->actionNormal->setChecked(false);
		ui->actionPosition->setChecked(false);
		ui->actionDepth->setChecked(false);
		ui->actionShadows->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugFinal();
	}
}

/**
 * @brief MainWindow::on_actionDiffuse_activated
 */
void MainWindow::on_actionDiffuse_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionNormal->setChecked(false);
		ui->actionPosition->setChecked(false);
		ui->actionDepth->setChecked(false);
		ui->actionShadows->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugDiffuse();
	}
}

/**
 * @brief MainWindow::on_actionNormal_activated
 */
void MainWindow::on_actionNormal_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);
		ui->actionPosition->setChecked(false);
		ui->actionDepth->setChecked(false);
		ui->actionShadows->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugNormal();
	}
}

/**
 * @brief MainWindow::on_actionPosition_activated
 */
void MainWindow::on_actionPosition_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);
		ui->actionNormal->setChecked(false);
		ui->actionDepth->setChecked(false);
		ui->actionShadows->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugPosition();
	}
}

/**
 * @brief MainWindow::on_actionDepth_activated
 */
void MainWindow::on_actionDepth_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);
		ui->actionNormal->setChecked(false);
		ui->actionPosition->setChecked(false);
		ui->actionShadows->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugDepth();
	}
}

/**
 * @brief MainWindow::on_actionShadows_toggled
 * @param checked
 */
void MainWindow::on_actionShadows_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);
		ui->actionNormal->setChecked(false);
		ui->actionPosition->setChecked(false);
		ui->actionDepth->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugShadows();
	}
}

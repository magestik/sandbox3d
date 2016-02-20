#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "DrawableSurface.h"

#include <QSettings>
#include <QDockWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileSystemWatcher>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget * pParent)
: QMainWindow(pParent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_pDrawable = new DrawableSurface(this);
	setCentralWidget(m_pDrawable);

	{
		m_pClearColorChooser = new QColorDialog(this);
		m_pClearColorChooser->hide();
		m_pClearColorChooser->setWindowTitle("Clear Color Chooser");
		connect(m_pClearColorChooser, SIGNAL(currentColorChanged(const QColor &)), m_pDrawable, SLOT(setClearColor(const QColor &)));
		m_pClearColorChooser->setCurrentColor(QColor::fromRgbF(0.5f, 0.5f, 0.5f));
	}

	{
		m_pFileChooser = new QFileDialog(this );
		m_pFileChooser->hide();
		m_pFileChooser->setWindowTitle("Import scene ...");
		m_pFileChooser->setAcceptMode(QFileDialog::AcceptOpen);
		m_pFileChooser->setFileMode(QFileDialog::ExistingFile);

		const char ext [] = "*.dae *.blend *.3ds *.ase *.obj *.ifc *.xgl *.zgl *.ply *.dxf *.lwo *.lws *.lxo *.stl *.x *.ac *.ms3d *.cob *.scn " /** Common interchange formats **/
							"*.bvh *.csm " /** Motion Capture Formats **/
							"*.xml *.irrmesh *.irr " /** Motion Capture Formats **/
							"*.mdl *.md2 *.md3 *.pk3 *.mdc *.md5 *.smd *.vta *.m3 *.3d " /** Game file formats **/
							"*.b3d *.q3d *.q3s *.nff *.nff *.off *.raw *.ter *.mdl *.hmp *.ndo "; /** Other file formats **/

		QString filter(ext);
		m_pFileChooser->setNameFilters(filter.split(' '));

		connect(m_pFileChooser, SIGNAL(fileSelected(const QString &)), m_pDrawable, SLOT(importScene(const QString &)));
	}

	{
		// TODO : make this work !
		m_pFileWatcher = new QFileSystemWatcher(this);
		m_pFileWatcher->addPath("data/shaders");
		connect(m_pFileWatcher, SIGNAL(directoryChanged(QString)), m_pDrawable, SLOT(reloadShader(QString)));
	}

	{
		m_pEnvSettingsWidget = new EnvSettingsWidget(this, m_pDrawable->m_renderer.environment);
	}

	QSettings settings;
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	restoreState(settings.value("mainWindowState").toByteArray());

	ui->statusbar->showMessage(tr("Ready"), 2000);
	ui->statusbar->addPermanentWidget(ui->cpu_time_text);
	ui->statusbar->addPermanentWidget(ui->gpu_time_text);
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
void MainWindow::closeEvent(QCloseEvent * pEvent)
{
	(void)pEvent;

	QSettings settings;
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
}

/**
 * @brief MainWindow::SetRenderTime
 * @param t
 */
void MainWindow::SetRenderTime(double cpu_time, double gpu_time)
{
	QString str1 = "CPU : "+ QString::number(cpu_time, 'f', 6 ) +" ms";
	ui->cpu_time_text->setText(str1);

	if (cpu_time < 4.0)
	{
		ui->cpu_time_text->setStyleSheet("* {color : black; }");
	}
	else
	{
		ui->cpu_time_text->setStyleSheet("* {color : red; }");
	}

	QString str2 = "GPU : "+ QString::number(gpu_time, 'f', 6 ) +" ms";
	ui->gpu_time_text->setText(str2);

	if (gpu_time < 16.0)
	{
		ui->gpu_time_text->setStyleSheet("* {color : black; }");
	}
	else
	{
		ui->gpu_time_text->setStyleSheet("* {color : red; }");
	}
}

/**
 * @brief MainWindow::SetStatus
 * @param str
 */
void MainWindow::SetStatus(const QString & str)
{
	ui->statusbar->showMessage(str, 2000);
}

/**
 * @brief MainWindow::on_actionImport_triggered
 */
void MainWindow::on_actionImport_triggered()
{
	m_pFileChooser->show();
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
 * @brief MainWindow::on_actionFinal_toggled
 * @param checked
 */
void MainWindow::on_actionFinal_toggled(bool checked)
{
	if (checked)
	{
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugFinal();
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

		ui->actionDepth->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugNormal();
	}
}

/**
 * @brief MainWindow::on_actionLightDiffuse_toggled
 * @param checked
 */
void MainWindow::on_actionLightDiffuse_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugDiffuseLights();
	}
}

/**
 * @brief MainWindow::on_actionLightSpecular_toggled
 */
void MainWindow::on_actionLightSpecular_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugSpecularLights();
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

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

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

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugShadows();
	}
}

/**
 * @brief MainWindow::on_actionLuminance1_toggled
 * @param checked
 */
void MainWindow::on_actionLuminance1_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance2->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugLuminance(1);
	}
}

/**
 * @brief MainWindow::on_actionLuminance2_toggled
 * @param checked
 */
void MainWindow::on_actionLuminance2_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);

		ui->actionBloom->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugLuminance(2);
	}
}

/**
 * @brief MainWindow::on_actionBloom_toggled
 * @param checked
 */
void MainWindow::on_actionBloom_toggled(bool checked)
{
	if (checked)
	{
		ui->actionFinal->setChecked(false);
		ui->actionDiffuse->setChecked(false);

		ui->actionDepth->setChecked(false);
		ui->actionNormal->setChecked(false);

		ui->actionLightDiffuse->setChecked(false);
		ui->actionLightSpecular->setChecked(false);

		ui->actionShadows->setChecked(false);

		ui->actionLuminance1->setChecked(false);
		ui->actionLuminance2->setChecked(false);

		static_cast<DrawableSurface*>(m_pDrawable)->DebugBloom();
	}
}

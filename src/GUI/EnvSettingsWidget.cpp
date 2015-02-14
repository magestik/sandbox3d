#include "EnvSettingsWidget.h"
#include "ui_EnvSettingsWidget.h"

#include "MainWindow.h"
#include "DrawableSurface.h"

#include <QColorDialog>

/**
 * @brief EnvSettingsWidget::EnvSettingsWidget
 * @param pParent
 */
EnvSettingsWidget::EnvSettingsWidget(MainWindow * pParent, EnvironmentSettings & env)
: QDockWidget(pParent)
, ui(new Ui::EnvSettingsWidget)
, environment(env)
{
	ui->setupUi(this);

	{
		m_pAmbientColorChooser = new QColorDialog(this);
		m_pAmbientColorChooser->hide();
		m_pAmbientColorChooser->setWindowTitle("Ambient Color Chooser");
		QObject::connect(m_pAmbientColorChooser, SIGNAL(currentColorChanged(const QColor &)), this, SLOT(setAmbientColor(const QColor &)));
		m_pAmbientColorChooser->setCurrentColor(QColor::fromRgbF(0.1f, 0.1f, 0.1f));
	}

	ui->ExtinctionSlider->setValue(10);
	ui->ScatteringSlider->setValue(10);
}

/**
 * @brief EnvSettingsWidget::~EnvSettingsWidget
 */
EnvSettingsWidget::~EnvSettingsWidget(void)
{
	delete ui;
}

/**
 * @brief EnvSettingsWidget::setAmbientColor
 * @param color
 */
void EnvSettingsWidget::setAmbientColor(const QColor & color)
{
	QString qss = QString("background-color: %1").arg(color.name());
	ui->AmbientColorButton->setStyleSheet(qss);

	environment.ambient.Color = vec3(color.redF(), color.greenF(), color.blueF());

	static_cast<MainWindow*>(parent())->m_pDrawable->update();
}

/**
 * @brief EnvSettingsWidget::on_AmbientColorButton_on_AmbientColorButton_pressed
 */
void EnvSettingsWidget::on_AmbientColorButton_pressed(void)
{
	m_pAmbientColorChooser->show();
}

/**
 * @brief EnvSettingsWidget::on_FogCheckbox_toggled
 * @param state
 */
void EnvSettingsWidget::on_FogCheckbox_toggled(bool state)
{
	if (state)
	{
		environment.enable(EnvironmentSettings::FOG);
		ui->FogParam_Group->setEnabled(true);
	}
	else
	{
		environment.disable(EnvironmentSettings::FOG);
		ui->FogParam_Group->setEnabled(false);
	}

	static_cast<MainWindow*>(parent())->m_pDrawable->update();
}

/**
 * @brief EnvSettingsWidget::on_ScatteringSlider_valueChanged
 * @param value
 */
void EnvSettingsWidget::on_ScatteringSlider_valueChanged(int value)
{
	environment.fog.Scattering = value / 1000.0f;

	if (environment.isEnabled(EnvironmentSettings::FOG))
	{
		static_cast<MainWindow*>(parent())->m_pDrawable->update();
	}
}

/**
 * @brief EnvSettingsWidget::on_ExtinctionSlider_valueChanged
 * @param value
 */
void EnvSettingsWidget::on_ExtinctionSlider_valueChanged(int value)
{
	environment.fog.Extinction = value / 1000.0f;

	if (environment.isEnabled(EnvironmentSettings::FOG))
	{
		static_cast<MainWindow*>(parent())->m_pDrawable->update();
	}
}

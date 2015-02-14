#pragma once

#include <QDockWidget>

namespace Ui {
	class EnvSettingsWidget;
}

class MainWindow;

class QColorDialog;

class EnvironmentSettings;

class EnvSettingsWidget : public QDockWidget
{
	Q_OBJECT

public:

	explicit EnvSettingsWidget(MainWindow * pParent, EnvironmentSettings & env);
	virtual ~EnvSettingsWidget();

private:

	Ui::EnvSettingsWidget * ui;

protected:

	QColorDialog * m_pAmbientColorChooser;

	EnvironmentSettings & environment;

private slots:

	void setAmbientColor(const QColor & color);

	void on_AmbientColorButton_pressed(void);
	void on_FogCheckbox_toggled(bool state);

	void on_ScatteringSlider_valueChanged(int value);
	void on_ExtinctionSlider_valueChanged(int value);
};

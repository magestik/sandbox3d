#include <QApplication>
#include <QGLFormat>
#include <QSplashScreen>

#include "GUI/MainWindow.h"

bool setDefaultFormatOpenGL(void)
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	return(true);
}

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	bool version = setDefaultFormatOpenGL();
	if (!version)
	{
		return(-1);
	}

	MainWindow w;
	w.show();

	return(app.exec());
}

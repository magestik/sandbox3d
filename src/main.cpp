#include <QApplication>
#include <QGLFormat>
#include <QSplashScreen>

#include "GUI/MainWindow.h"

QSplashScreen * g_pSplashScreen = nullptr;

bool setDefaultFormatOpenGL(void)
{
	QGLFormat fmt;
	fmt.setDoubleBuffer(true);
	fmt.setDirectRendering(true);
	fmt.setRgba(true);
	fmt.setStencil(false);
	fmt.setDepth(false);
	fmt.setAlpha(false);
	fmt.setVersion(3, 3);
	fmt.setProfile(QGLFormat::CoreProfile);
	QGLFormat::setDefaultFormat(fmt);

	return(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_3);
}

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	bool version = setDefaultFormatOpenGL();
	if (!version)
	{
		return(-1);
	}


	QPixmap pixmap("splash.png");
	QSplashScreen splash(pixmap);
	splash.setWindowFlags(splash.windowFlags() | Qt::WindowStaysOnTopHint);
	splash.show();

	g_pSplashScreen = &splash;

	app.processEvents();

	MainWindow w;
	w.show();

	splash.finish(&w);

	return(app.exec());
}

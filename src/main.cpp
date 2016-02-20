#include <QApplication>
#include <QGLFormat>
#include <QSplashScreen>

#include "GUI/MainWindow.h"

void getToto()
{

}
void setToto()
{

}
int test [[ri::name("Test", "Test Variable"), setter(getToto), getter(setToto)]];

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

	MainWindow w;
	w.show();

	return(app.exec());
}

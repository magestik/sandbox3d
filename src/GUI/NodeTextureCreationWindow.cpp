#include "NodeTextureCreationWindow.h"
#include "ui_NodeTextureCreationWindow.h"

/**
 * @brief NodeTextureCreationWindow::NodeTextureCreationWindow
 * @param pParent
 */
NodeTextureCreationWindow::NodeTextureCreationWindow(QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeTextureCreationWindow)
{
	ui->setupUi(this);
}

/**
 * @brief NodeTextureCreationWindow::~NodeTextureCreationWindow
 */
NodeTextureCreationWindow::~NodeTextureCreationWindow()
{
	// nothing here
}

/**
 * @brief NodeTextureCreationWindow::getCurrentFormat
 * @return
 */
unsigned int NodeTextureCreationWindow::getCurrentFormat(void) const
{
	return(0);
}

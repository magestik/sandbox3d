#include "NodeCreationWindow.h"
#include "ui_NodeCreationWindow.h"

/**
 * @brief NodeCreationWindow::NodeCreationWindow
 * @param pParent
 */
NodeCreationWindow::NodeCreationWindow(const std::vector<NodeDescriptor> & aDescriptors, QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeCreationWindow)
{
	ui->setupUi(this);

	for (const NodeDescriptor & desc : aDescriptors)
	{
		ui->comboBox->addItem(QString(desc.name.c_str()), QVariant::fromValue((void*)&desc));
	}
}

/**
 * @brief NodeCreationWindow::~NodeCreationWindow
 */
NodeCreationWindow::~NodeCreationWindow()
{
	// nothing here
}

/**
 * @brief NodeCreationWindow::getCurrentDescriptor
 * @return
 */
NodeDescriptor * NodeCreationWindow::getCurrentDescriptor(void)
{
	QVariant variant = ui->comboBox->itemData(ui->comboBox->currentIndex());
	return(static_cast<NodeDescriptor*>(variant.value<void*>()));
}

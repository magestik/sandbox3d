#include "NodeOperationCreationWindow.h"
#include "ui_NodeOperationCreationWindow.h"

/**
 * @brief NodeOperationCreationWindow::NodeOperationCreationWindow
 * @param pParent
 */
NodeOperationCreationWindow::NodeOperationCreationWindow(const std::vector<NodeDescriptor> & aDescriptors, QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeOperationCreationWindow)
{
	ui->setupUi(this);

	for (const NodeDescriptor & desc : aDescriptors)
	{
		ui->comboBox->addItem(QString(desc.name.c_str()), QVariant::fromValue((void*)&desc));
	}
}

/**
 * @brief NodeOperationCreationWindow::~NodeOperationCreationWindow
 */
NodeOperationCreationWindow::~NodeOperationCreationWindow()
{
	// nothing here
}

/**
 * @brief NodeOperationCreationWindow::getCurrentDescriptor
 * @return
 */
NodeDescriptor * NodeOperationCreationWindow::getCurrentDescriptor(void)
{
	QVariant variant = ui->comboBox->itemData(ui->comboBox->currentIndex());
	return(static_cast<NodeDescriptor*>(variant.value<void*>()));
}

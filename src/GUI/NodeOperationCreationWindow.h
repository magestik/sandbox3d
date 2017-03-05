#pragma once

#include "NodeDescription.h"

#include <QDialog>

namespace Ui {
	class NodeOperationCreationWindow;
}

class NodeOperationCreationWindow : public QDialog
{
	Q_OBJECT

public:

	explicit	NodeOperationCreationWindow		(const std::vector<NodeDescriptor> & aDescriptors, QWidget * parent = nullptr);
	virtual		~NodeOperationCreationWindow	(void);

	NodeDescriptor * getCurrentDescriptor();

private:

	Ui::NodeOperationCreationWindow * ui;

protected:

};

#pragma once

#include "NodeDescription.h"

#include <QDialog>

namespace Ui {
	class NodeCreationWindow;
}

class NodeCreationWindow : public QDialog
{
	Q_OBJECT

public:

	explicit	NodeCreationWindow		(const std::vector<NodeDescriptor> & aDescriptors, QWidget * parent = nullptr);
	virtual		~NodeCreationWindow		(void);

	NodeDescriptor * getCurrentDescriptor();

private:

	Ui::NodeCreationWindow * ui;

protected:

};

#pragma once

#include "NodeDescription.h"

#include <QDialog>

namespace Ui {
	class NodeTextureCreationWindow;
}

class NodeTextureCreationWindow : public QDialog
{
	Q_OBJECT

public:

	explicit	NodeTextureCreationWindow		(QWidget * parent = nullptr);
	virtual		~NodeTextureCreationWindow		(void);

	unsigned int getCurrentFormat				(void) const;
private:

	Ui::NodeTextureCreationWindow * ui;

protected:

};

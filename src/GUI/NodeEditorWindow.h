#pragma once

#include <QDialog>

class GraphicsNodeView;
class GraphicsNodeScene;

namespace Ui {
	class NodeEditorWindow;
}

class NodeEditorWindow : public QDialog
{
	Q_OBJECT

public:

	explicit	NodeEditorWindow		(QWidget * parent = 0);
	virtual		~NodeEditorWindow		(void);

	void		closeEvent				(QCloseEvent * event);

private:

	Ui::NodeEditorWindow * ui;

protected:

	GraphicsNodeView *		m_pView;
	GraphicsNodeScene *		m_pScene;
};

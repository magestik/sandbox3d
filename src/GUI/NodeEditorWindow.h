#pragma once

#include "NodeDescription.h"

#include <QDialog>

class QMenuBar;
class QStatusBar;
class QMenu;

class NodeCreationWindow;

class GraphicsNodeView;
class GraphicsNodeScene;

namespace Ui {
	class NodeEditorWindow;
}

class NodeEditorWindow : public QDialog
{
	Q_OBJECT

public:

	explicit	NodeEditorWindow		(QWidget * parent = nullptr);
	virtual		~NodeEditorWindow		(void);

	void		closeEvent				(QCloseEvent * event) Q_DECL_OVERRIDE;
	void		contextMenuEvent		(QContextMenuEvent * event) Q_DECL_OVERRIDE;

protected:

	void		loadNodeDescriptors		(void);

private:

	Ui::NodeEditorWindow * ui;

protected:

	QMenuBar *				m_pMenuBar;
	QStatusBar *			m_pStatusBar;

	GraphicsNodeView *		m_pView;
	GraphicsNodeScene *		m_pScene;

	QMenu *					m_pContextMenuScene;
	QMenu *					m_pContextMenuNode;

	NodeCreationWindow *	m_pNodeCreationWindow;

	std::vector<NodeDescriptor> m_aNodeDescriptors;

private slots:

	void on_actionAddNode_triggered();
	void on_actionDeleteNode_triggered();

	void createNodeFromWindow();

};

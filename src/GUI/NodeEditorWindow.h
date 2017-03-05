#pragma once

#include "NodeDescription.h"

#include <QDialog>

class QMenuBar;
class QStatusBar;
class QMenu;

class NodeCreationWindow;

class GraphicsNodeView;
class GraphicsNodeScene;
class GraphicsNode;

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

	void				loadNodeDescriptors		(void);

	bool				loadGraph				(void);
	bool				saveGraph				(void);
	void				createDefaultNodes		(void);
	GraphicsNode *		createNode				(const NodeDescriptor & desc);

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

	NodeDescriptor					m_PresentNodeDescriptor;
	std::vector<NodeDescriptor>		m_aNodeDescriptors;

	std::map<const GraphicsNode*, const NodeDescriptor*> m_mapNode;

private slots:

	void on_actionSave_triggered();

	void on_actionCreateUserDefinedNode_triggered();
	void on_actionRemoveNode_triggered();

	void createNodeFromWindow();

};

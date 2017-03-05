#pragma once

#include "NodeDescription.h"

#include <QDialog>

class QMenuBar;
class QStatusBar;
class QMenu;

class NodeOperationCreationWindow;
class NodeTextureCreationWindow;

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
	GraphicsNode *		createPresentNode		(void);
	GraphicsNode *		createOperationNode		(const NodeDescriptor & desc);
	GraphicsNode *		createTextureNode		(unsigned int fomat, unsigned int width, unsigned int height);

private:

	Ui::NodeEditorWindow * ui;

protected:

	QMenuBar *				m_pMenuBar;
	QStatusBar *			m_pStatusBar;

	GraphicsNodeView *		m_pView;
	GraphicsNodeScene *		m_pScene;

	QMenu *					m_pContextMenuScene;
	QMenu *					m_pContextMenuNode;

	NodeOperationCreationWindow *	m_pNodeOperationCreationWindow;
	NodeTextureCreationWindow *		m_pNodeTextureCreationWindow;

	std::vector<NodeDescriptor>		m_aNodeDescriptors;

	std::map<const GraphicsNode*, const NodeDescriptor*> m_mapNode;
	std::map<const GraphicsNode*, std::string> m_mapNodeType;

private slots:

	void on_actionSave_triggered();

	void on_actionCreateUserDefinedNode_triggered();
	void on_actionCreateTextureNode_triggered();
	void on_actionRemoveNode_triggered();

	void createOperationNodeFromDialog();
	void createTextureNodeFromDialog();

};

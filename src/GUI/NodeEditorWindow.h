#pragma once

#include <QDialog>

class QMenuBar;
class QStatusBar;
class QMenu;

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

	void		closeEvent				(QCloseEvent * event) Q_DECL_OVERRIDE;
	void		contextMenuEvent		(QContextMenuEvent * event) Q_DECL_OVERRIDE;

private:

	Ui::NodeEditorWindow * ui;

protected:

	QMenuBar *				m_pMenuBar;
	QStatusBar *			m_pStatusBar;

	GraphicsNodeView *		m_pView;
	GraphicsNodeScene *		m_pScene;

	QMenu *					m_pContextMenuScene;
	QMenu *					m_pContextMenuNode;

private slots:

	void on_actionAddNode_triggered();

	void on_actionAddInput_triggered();
	void on_actionAddOutput_triggered();

	void on_actionDelete_triggered();

};

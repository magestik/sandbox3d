#include "NodeEditorWindow.h"
#include "ui_NodeEditorWindow.h"

#include <QVBoxLayout>
#include <QStatusBar>

#include <graphicsnodescene.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnode.hpp>
#include <graphicsbezieredge.hpp>
#include <qobjectnode.hpp>

/**
 * @brief NodeEditorWindow::NodeEditorWindow
 * @param pParent
 */
NodeEditorWindow::NodeEditorWindow(QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeEditorWindow)
, m_pView(nullptr)
, m_pScene(nullptr)
{
	setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	setMinimumWidth(640);
	setMinimumHeight(480);

	// create and configure scene
	m_pScene = new GraphicsNodeScene(this);
	m_pScene->setSceneRect(-32000, -32000, 64000, 64000);

	// view setup
	m_pView = new GraphicsNodeView(this);
	m_pView->setScene(m_pScene);


	QVBoxLayout * pLayout = new QVBoxLayout(this);
	pLayout->setMargin(0);
	pLayout->addWidget(m_pView);

	QStatusBar * statusBar = new QStatusBar;
	pLayout->addWidget(statusBar);

	setLayout(pLayout);
}

/**
 * @brief MainWindow::~MainWindow
 */
NodeEditorWindow::~NodeEditorWindow()
{
	// nothing here
}

/**
 * @brief NodeEditorWindow::closeEvent
 * @param event
 */
void NodeEditorWindow::closeEvent(QCloseEvent * pEvent)
{
	// nothing here
}

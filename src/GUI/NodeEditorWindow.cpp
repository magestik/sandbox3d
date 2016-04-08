#include "NodeEditorWindow.h"
#include "ui_NodeEditorWindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QContextMenuEvent>
#include <QAction>

#include <graphicsnodescene.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnode.hpp>
#include <graphicsbezieredge.hpp>
#include <qobjectnode.hpp>

#include <assert.h>

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
	ui->setupUi(this);

	setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	setMinimumWidth(640);
	setMinimumHeight(480);

	// Menu Bar
	{
		m_pMenuBar = new QMenuBar;
		ui->layout->addWidget(m_pMenuBar);
	}

	// Node Widget
	{
		// create and configure scene
		m_pScene = new GraphicsNodeScene(this);
		m_pScene->setSceneRect(-32000, -32000, 64000, 64000);

		// view setup
		m_pView = new GraphicsNodeView(this);
		m_pView->setScene(m_pScene);

		ui->layout->addWidget(m_pView);
	}

	// Status Bar
	{
		m_pStatusBar = new QStatusBar;
		ui->layout->addWidget(m_pStatusBar);
	}

	// Centext menu
	{
		m_pContextMenuScene = new QMenu(this);
		m_pContextMenuScene->addAction(ui->actionAddNode);

		m_pContextMenuNode = new QMenu(this);
		m_pContextMenuNode->addAction(ui->actionAddInput);
		m_pContextMenuNode->addAction(ui->actionAddOutput);
	}
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

/**
 * @brief NodeEditorWindow::contextMenuEvent
 * @param event
 */
void NodeEditorWindow::contextMenuEvent(QContextMenuEvent * pEvent)
{
	QGraphicsItem * item = m_pView->itemAt(pEvent->pos());

	m_pScene->clearSelection();

	if (item)
	{
		switch (item->type())
		{
			case GraphicsNodeItemTypes::TypeNode:
			{
				item->setSelected(true);

				m_pContextMenuNode->exec(pEvent->globalPos());
			}
			break;
		}
	}
	else
	{
		m_pContextMenuScene->exec(pEvent->globalPos());
	}
}

/**
 * @brief NodeEditorWindow::on_actionAddNode_triggered
 */
void NodeEditorWindow::on_actionAddNode_triggered()
{
	auto n = new GraphicsNode();

	n->setTitle(QString("Node"));

	m_pScene->addItem(n);
}

/**
 * @brief NodeEditorWindow::on_actionAddInput_triggered
 */
void NodeEditorWindow::on_actionAddInput_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();
	assert(list.count() == 1);

	QGraphicsItem * item = list.at(0);
	assert(item->type() == GraphicsNodeItemTypes::TypeNode);

	GraphicsNode * node = static_cast<GraphicsNode*>(item);
	node->add_sink("input");
}

/**
 * @brief NodeEditorWindow::on_actionAddOutput_triggered
 */
void NodeEditorWindow::on_actionAddOutput_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();
	assert(list.count() == 1);

	QGraphicsItem * item = list.at(0);
	assert(item->type() == GraphicsNodeItemTypes::TypeNode);

	GraphicsNode * node = static_cast<GraphicsNode*>(item);
	node->add_source("output");
}

/**
 * @brief NodeEditorWindow::on_actionDelete_triggered
 */
void NodeEditorWindow::on_actionDelete_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();

	for (QGraphicsItem * item : list)
	{
		if (item->type() == GraphicsNodeItemTypes::TypeNode)
		{
			m_pScene->removeItem(item);
			delete item;
		}
	}
}

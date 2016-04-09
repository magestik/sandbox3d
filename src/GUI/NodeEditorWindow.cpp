#include "NodeEditorWindow.h"
#include "ui_NodeEditorWindow.h"

#include "NodeCreationWindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QContextMenuEvent>
#include <QAction>
#include <QDir>
#include <QMessageBox>

#include <graphicsnodescene.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnodeview.hpp>
#include <graphicsnode.hpp>
#include <graphicsbezieredge.hpp>
#include <qobjectnode.hpp>

#include <assert.h>

#include <jansson.h>

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
		m_pContextMenuNode->addAction(ui->actionDeleteNode);
	}

	loadNodeDescriptors();

	// Node Creation Window
	{
		m_pNodeCreationWindow = new NodeCreationWindow(m_aNodeDescriptors, this);
		connect(m_pNodeCreationWindow, SIGNAL(accepted()), this, SLOT(createNodeFromWindow()));
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

void NodeEditorWindow::loadNodeDescriptors(void)
{
	QDir dir("data/nodes");
	QStringList list = dir.entryList(QDir::Files);

	bool successful = true;

	for (QString & filename : list)
	{
		NodeDescriptor desc;
		bool loading = desc.loadFromFile(dir.filePath(filename).toStdString());

		if (loading)
		{
			m_aNodeDescriptors.push_back(desc);
		}
		else
		{
			successful = false;
		}
	}

	if (!successful)
	{
		int ret = QMessageBox::warning(this, tr("Sandbox 3D"), tr("All Nodes were not loaded successfully !"));
	}
}

/**
 * @brief NodeEditorWindow::on_actionAddNode_triggered
 */
void NodeEditorWindow::on_actionAddNode_triggered()
{
	m_pNodeCreationWindow->show();
}

/**
 * @brief NodeEditorWindow::on_actionDelete_triggered
 */
void NodeEditorWindow::on_actionDeleteNode_triggered(void)
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

/**
 * @brief NodeEditorWindow::createNodeFromWindow
 */
void NodeEditorWindow::createNodeFromWindow()
{
	NodeDescriptor * pDesc = m_pNodeCreationWindow->getCurrentDescriptor();

	if (pDesc)
	{
		GraphicsNode * n = new GraphicsNode();
		n->setTitle(QString(pDesc->name.c_str()));

		for (NodeDescriptor::Input & input : pDesc->inputs)
		{
			n->add_sink(QString(input.name.c_str()));
		}

		for (NodeDescriptor::Output & output : pDesc->outputs)
		{
			n->add_source(QString(output.name.c_str()));
		}

		m_pScene->addItem(n);
	}
}

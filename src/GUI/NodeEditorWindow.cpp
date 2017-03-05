#include "NodeEditorWindow.h"
#include "ui_NodeEditorWindow.h"

#include "NodeOperationCreationWindow.h"
#include "NodeTextureCreationWindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QContextMenuEvent>
#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QLabel>

#include <graphicsnodescene.hpp>
#include <graphicsnodeview.hpp>

#include <graphicsnode.hpp>
#include <graphicsbezieredge.hpp>
#include <graphicsnodesocket.hpp>

#include <qobjectnode.hpp>

#include <jansson.h>

#include <assert.h>

#include <algorithm>

#define GRAPH_FILE_PATH "data/render-graph.json"

#define SCENE_SIZE_X 20000.0
#define SCENE_SIZE_Y 20000.0

/**
 * @brief Constructor
 * @param Parent widget (usually the main window)
 */
NodeEditorWindow::NodeEditorWindow(QWidget * pParent)
: QDialog(pParent)
, ui(new Ui::NodeEditorWindow)
, m_pView(nullptr)
, m_pScene(nullptr)
{
	ui->setupUi(this); // created with Qt Designer

	// Custom Widgets
	m_pMenuBar				= new QMenuBar(this); // can't add a Menu bar from Designer
	m_pStatusBar			= new QStatusBar(this); // can't add a Status bar from Designer

	m_pScene				= new GraphicsNodeScene(this);
	m_pView					= new GraphicsNodeView(m_pScene, this);

	ui->layout->addWidget(m_pMenuBar);
	ui->layout->addWidget(m_pView);
	ui->layout->addWidget(m_pStatusBar);

	// Context Menus
	m_pContextMenuScene		= new QMenu(this);
	m_pContextMenuNode		= new QMenu(this);

	// Window
	setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	// Node Widget
	m_pScene->setSceneRect(-(SCENE_SIZE_X*0.5), -(SCENE_SIZE_Y*0.5), SCENE_SIZE_X, SCENE_SIZE_Y);


	// Load descriptors (to create nodes)
	{
		loadNodeDescriptors();
	}

	//
	// Create windows
	{
		m_pNodeOperationCreationWindow	= new NodeOperationCreationWindow(m_aNodeDescriptors, this);
		m_pNodeTextureCreationWindow = new NodeTextureCreationWindow(this);
	}

	// Actions
	m_pContextMenuScene->addAction(ui->actionCreateUserDefinedNode);
	m_pContextMenuScene->addAction(ui->actionCreateTextureNode);
	m_pContextMenuNode->addAction(ui->actionRemoveNode);

	addAction(ui->actionRemoveNode);
	addAction(ui->actionSave);

	connect(m_pNodeOperationCreationWindow, SIGNAL(accepted()), this, SLOT(createOperationNodeFromDialog()));
	connect(m_pNodeTextureCreationWindow, SIGNAL(accepted()), this, SLOT(createTextureNodeFromDialog()));

	// Load current Render Graph
	if (!loadGraph())
	{
		createPresentNode();
	}
}

/**
 * @brief Destructor
 */
NodeEditorWindow::~NodeEditorWindow(void)
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
 * @brief NodeEditorWindow::loadNodeDescriptors
 */
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
		assert(0 == ret);
	}
}

/**
 * @brief NodeEditorWindow::loadGraph
 * @return
 */
bool NodeEditorWindow::loadGraph(void)
{
	json_error_t err;
	json_t * pRoot = json_load_file(GRAPH_FILE_PATH, 0, &err);

	if (!pRoot)
	{
		return(false);
	}

	// Get the graph
	json_t * pGraph = json_object_get(pRoot, "graph");

	// Type
	json_t * pGraphType = json_object_get(pGraph, "type");
	assert(nullptr != pGraphType);
	assert(!strcmp("RenderGraph", json_string_value(pGraphType)));

	// Label
	json_t * pGraphLabel = json_object_get(pGraph, "label");
	assert(nullptr != pGraphLabel);

	// Metadata
	json_t * pGraphMetadata = json_object_get(pGraph, "metadata");
	assert(nullptr != pGraphLabel);

	// Nodes
	json_t * pGraphNodes = json_object_get(pGraph, "nodes");
	assert(nullptr != pGraphNodes);

	std::map<uintptr_t, GraphicsNode *> mapUID;

	{
		size_t index;
		json_t * pNode;

		json_array_foreach(pGraphNodes, index, pNode)
		{
			// ID
			json_t * pNodeID = json_object_get(pNode, "id");
			uintptr_t uid = json_integer_value(pNodeID);
			assert(0 != uid);

			// Type
			json_t * pNodeType = json_object_get(pNode, "type");
			const char * strType = json_string_value(pNodeType);

			// Label
			json_t * pNodeLabel = json_object_get(pNode, "label");
			assert(nullptr != pNodeLabel);

			// Metadata
			json_t * pNodeMetada = json_object_get(pNode, "metadata");
			assert(nullptr != pNodeMetada);

			json_t * pNodePosX = json_object_get(pNodeMetada, "xloc");
			assert(nullptr != pNodePosX);

			json_t * pNodePosY = json_object_get(pNodeMetada, "yloc");
			assert(nullptr != pNodePosY);

			// Create Node
			GraphicsNode * n = nullptr;

			if (!strcmp("operation", strType))
			{
				json_t * pNodeSubType = json_object_get(pNodeMetada, "subtype");
				assert(nullptr != pNodeSubType);
				const char * strSubType = json_string_value(pNodeSubType);

				std::vector<NodeDescriptor>::iterator it = std::find(m_aNodeDescriptors.begin(), m_aNodeDescriptors.end(), strSubType);
				assert(it != m_aNodeDescriptors.end());

				n = createOperationNode(*it);
			}
			else if (!strcmp("texture", strType))
			{
				json_t * pNodeFormat = json_object_get(pNodeMetada, "format");
				assert(nullptr != pNodeFormat);

				n = createTextureNode(json_integer_value(pNodeFormat), 1024, 1024);
			}
			else if (!strcmp("present", strType))
			{
				n = createPresentNode();
			}
			else
			{
				assert(false);
			}

			n->setPos(json_real_value(pNodePosX), json_real_value(pNodePosY));
			n->setTitle(json_string_value(pNodeLabel)); // currently useless

			mapUID[uid] = n;
		}
	}

	// Edges
	json_t * pGraphEdges = json_object_get(pGraph, "edges");
	assert(nullptr != pGraphEdges);

	{
		size_t index;
		json_t * pNode;

		json_array_foreach(pGraphEdges, index, pNode)
		{
			// Source
			json_t * pNodeEdgeSource = json_object_get(pNode, "source");
			uintptr_t source_uid = json_integer_value(pNodeEdgeSource);

			// Target
			json_t * pNodeEdgeTarget = json_object_get(pNode, "target");
			uintptr_t target_uid = json_integer_value(pNodeEdgeTarget);

			// Directed
			json_t * pNodeEdgeDirected = json_object_get(pNode, "directed");
			assert(json_boolean_value(pNodeEdgeDirected));

			// Metadata
			json_t * pNodeEdgeMetada = json_object_get(pNode, "metadata");
			assert(nullptr != pNodeEdgeMetada);

			json_t * pNodeEdgeSourceId = json_object_get(pNodeEdgeMetada, "source_id");
			assert(nullptr != pNodeEdgeSourceId);

			json_t * pNodeEdgeTargetId = json_object_get(pNodeEdgeMetada, "target_id");
			assert(nullptr != pNodeEdgeTargetId);

			// Create Edge
			GraphicsNode * pSourceNode = mapUID[source_uid];
			assert(nullptr != pSourceNode);

			GraphicsNode * pTargetNode = mapUID[target_uid];
			assert(nullptr != pTargetNode);

			GraphicsDirectedEdge * pEdge = new GraphicsBezierEdge();
			pEdge->connect(pSourceNode, json_integer_value(pNodeEdgeSourceId), pTargetNode, json_integer_value(pNodeEdgeTargetId));

			m_pScene->addItem(pEdge);
		}
	}

	return(true);
}

/**
 * @brief NodeEditorWindow::saveGraph
 * @return
 */
bool NodeEditorWindow::saveGraph(void)
{
	json_t * pRoot = json_object();

	// Add the graph
	json_t * pGraph = json_object();
	json_object_set(pRoot, "graph", pGraph);

	// Type
	json_t * pGraphType = json_string("RenderGraph");
	json_object_set(pGraph, "type", pGraphType);

	// Label
	json_t * pGraphLabel = json_string("Unnamed Render Graph");
	json_object_set(pGraph, "label", pGraphLabel);

	// Metadata
	json_t * pGraphMetadata = json_object();
	json_object_set(pGraph, "metadata", pGraphMetadata);

	// Add the nodes/edges
	json_t * pGraphNodes = json_array();
	json_object_set(pGraph, "nodes", pGraphNodes);

	json_t * pGraphEdges = json_array();
	json_object_set(pGraph, "edges", pGraphEdges);

	QList<QGraphicsItem*> items = m_pScene->items();

	for (QGraphicsItem * item : items)
	{
		switch (item->type())
		{
			case GraphicsNodeItemTypes::TypeNode:
			{
				GraphicsNode * pNodeItem = static_cast<GraphicsNode*>(item);
				assert(nullptr != pNodeItem);

				std::string & strNodeType = m_mapNodeType[pNodeItem];

				json_t * pNode = json_object();
				json_array_append(pGraphNodes, pNode);

				// ID
				json_t * pNodeID = json_integer(uintptr_t(pNodeItem));
				json_object_set(pNode, "id", pNodeID);

				// Type
				json_t * pNodeType = json_string(m_mapNodeType[pNodeItem].c_str());
				json_object_set(pNode, "type", pNodeType);

				// Label
				json_t * pNodeLabel = json_string(pNodeItem->getTitle().toLocal8Bit());
				json_object_set(pNode, "label", pNodeLabel);

				// Metadata
				json_t * pNodeMetada = json_object();
				json_object_set(pNode, "metadata", pNodeMetada);

				if (strNodeType == "operation")
				{
					json_t * pNodeSubType = json_string(m_mapNode[pNodeItem]->identifier.c_str());
					json_object_set(pNodeMetada, "subtype", pNodeSubType);
				}
				else if (strNodeType == "texture")
				{
					json_t * pNodeSubType = json_integer(0);
					json_object_set(pNodeMetada, "format", pNodeSubType);
				}

				json_t * pNodePosX = json_real(pNodeItem->pos().x());
				json_object_set(pNodeMetada, "xloc", pNodePosX);

				json_t * pNodePosY = json_real(pNodeItem->pos().y());
				json_object_set(pNodeMetada, "yloc", pNodePosY);

				json_t * pNodeSizeX = json_real(pNodeItem->width());
				json_object_set(pNodeMetada, "width", pNodeSizeX);

				json_t * pNodeSizeY = json_real(pNodeItem->height());
				json_object_set(pNodeMetada, "height", pNodeSizeY);
			}
			break;

			case GraphicsNodeItemTypes::TypeBezierEdge:
			{
				GraphicsBezierEdge * pEdgeItem = static_cast<GraphicsBezierEdge*>(item);
				assert(nullptr != pEdgeItem);

				json_t * pEdge = json_object();
				json_array_append(pGraphEdges, pEdge);

				// Source
				json_t * pEdgeSource = json_integer(uintptr_t(pEdgeItem->getSource()->parentItem()));
				json_object_set(pEdge, "source", pEdgeSource);

				// Target
				json_t * pEdgeTarget = json_integer(uintptr_t(pEdgeItem->getSink()->parentItem()));
				json_object_set(pEdge, "target", pEdgeTarget);

				// Directed
				json_t * pEdgeDirected = json_true();
				json_object_set(pEdge, "directed", pEdgeDirected);

				// Metadata
				json_t * pEdgeMetada = json_object();
				json_object_set(pEdge, "metadata", pEdgeMetada);

				json_t * pEdgeSourceId = json_integer(pEdgeItem->getSource()->getIndex());
				json_object_set(pEdgeMetada, "source_id", pEdgeSourceId);

				json_t * pEdgeTargetId = json_integer(pEdgeItem->getSink()->getIndex());
				json_object_set(pEdgeMetada, "target_id", pEdgeTargetId);
			}
			break;

			default:
			{
				// don't save anything else
			}
		}
	}

	int success = json_dump_file(pRoot, GRAPH_FILE_PATH, 0);

	return(0 == success);
}

/**
 * @brief NodeEditorWindow::createDefaultNodes
 */
GraphicsNode * NodeEditorWindow::createPresentNode(void)
{
	GraphicsNode * n = new GraphicsNode();

	n->setTitle(QString("Present"));

	n->add_sink(QString("Backbuffer"), nullptr, 0);

	m_pScene->addItem(n);

	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "present"));

	return(n);
}

/**
 * @brief NodeEditorWindow::createNode
 * @param desc
 */
GraphicsNode * NodeEditorWindow::createOperationNode(const NodeDescriptor & desc)
{
	GraphicsNode * n = new GraphicsNode();

	n->setTitle(QString(desc.name.c_str()));

	int input_index = 0;

	for (const NodeDescriptor::Input & input : desc.inputs)
	{
		n->add_sink(QString(input.name.c_str()), nullptr, input_index);
		++input_index;
	}

	int output_index = 0;

	for (const NodeDescriptor::Output & output : desc.outputs)
	{
		n->add_source(QString(output.name.c_str()), nullptr, output_index);
		++output_index;
	}

	m_pScene->addItem(n);

	m_mapNode.insert(std::pair<const GraphicsNode*, const NodeDescriptor*>(n, &desc));
	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "operation"));

	return(n);
}
/**
 * @brief NodeEditorWindow::createNode
 * @param desc
 */
GraphicsNode * NodeEditorWindow::createTextureNode(unsigned int format, unsigned int width, unsigned int height)
{
	GraphicsNode * n = new GraphicsNode();

	n->setTitle(QString("Texture"));

	n->add_sink(QString(), nullptr, 0);
	n->add_source(QString(), nullptr, 0);

	QPixmap * pPixmap = new QPixmap(128, 128);
	pPixmap->fill(Qt::red);

	QLabel * pLabel = new QLabel();
	pLabel->setPixmap(*pPixmap);

	n->setCentralWidget(pLabel);

	m_pScene->addItem(n);

	//m_mapNode.insert(std::pair<const GraphicsNode*, const NodeDescriptor*>(n, &desc));
	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "texture"));

	return(n);
}

/**
 * @brief NodeEditorWindow::on_actionSave_triggered
 */
void NodeEditorWindow::on_actionSave_triggered(void)
{
	saveGraph();
}

/**
 * @brief NodeEditorWindow::actionCreateUserDefinedNode
 */
void NodeEditorWindow::on_actionCreateUserDefinedNode_triggered(void)
{
	m_pNodeOperationCreationWindow->show();
}

/**
 * @brief NodeEditorWindow::actionCreateUserDefinedNode
 */
void NodeEditorWindow::on_actionCreateTextureNode_triggered(void)
{
	m_pNodeTextureCreationWindow->show();
}

/**
 * @brief NodeEditorWindow::on_actionRemoveNode_triggered
 */
void NodeEditorWindow::on_actionRemoveNode_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();

	for (QGraphicsItem * item : list)
	{
		if (item->type() == GraphicsNodeItemTypes::TypeNode)
		{
			item->setSelected(false);
			m_pScene->removeItem(item);
		}
	}
}

/**
 * @brief NodeEditorWindow::createNodeFromWindow
 */
void NodeEditorWindow::createOperationNodeFromDialog(void)
{
	NodeDescriptor * pDesc = m_pNodeOperationCreationWindow->getCurrentDescriptor();

	if (pDesc)
	{
		createOperationNode(*pDesc);
	}
}

/**
 * @brief NodeEditorWindow::createNodeFromWindow
 */
void NodeEditorWindow::createTextureNodeFromDialog(void)
{
	unsigned int format = m_pNodeTextureCreationWindow->getCurrentFormat();

	createTextureNode(format, 1024, 1024);
}

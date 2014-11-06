#include "MeshListWidget.h"
#include "ui_meshlistwidget.h"

#include "Rendering/Rendering.h"

#include "MainWindow.h"

#include <QGLWidget>

/**
 * @brief MeshListWidget::MeshListWidget
 * @param parent
 */
MeshListWidget::MeshListWidget(QWidget *parent) :
QDockWidget(parent),
ui(new Ui::MeshListWidget)
{
	ui->setupUi(this);

	connect(ui->listWidget->selectionModel(),
	   SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	   this, SLOT(handleSelectionChanged(QItemSelection)));
}

/**
 * @brief MeshListWidget::~MeshListWidget
 */
MeshListWidget::~MeshListWidget()
{
	delete ui;
}

/**
 * @brief MeshListWidget::addMesh
 * @param str
 */
void MeshListWidget::addMesh(const QString & str)
{
	ui->listWidget->addItem(str);
}

/**
 * @brief MeshListWidget::currentChanged
 * @param current
 * @param previous
 */
void MeshListWidget::handleSelectionChanged(const QItemSelection& selection)
{
	QModelIndex index = selection.indexes().first();
	QListWidgetItem * row = ui->listWidget->item(index.row());

	static_cast<MainWindow*>(parent())->m_pDrawable->update();
}

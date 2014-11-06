#ifndef MESHLISTWIDGET_H
#define MESHLISTWIDGET_H

#include <QDockWidget>

class QItemSelection;

namespace Ui {
	class MeshListWidget;
}

class MeshListWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit MeshListWidget(QWidget *parent = 0);
	~MeshListWidget();

	void addMesh(const QString & str);

private:

	Ui::MeshListWidget * ui;

private slots:

	 void handleSelectionChanged(const QItemSelection& selection);

};


#endif // MESHLISTWIDGET_H

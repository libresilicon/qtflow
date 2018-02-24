#ifndef QTREEVIEWDRAGDROP_H
#define QTREEVIEWDRAGDROP_H

#include <QTreeView>
#include <QDragEnterEvent>

class QTreeViewDragDrop : public QTreeView
{
	Q_OBJECT
public:
	explicit QTreeViewDragDrop(QWidget *parent = nullptr);

signals:

public slots:
	void dragEnterEvent(QDragEnterEvent *event);

};

#endif // QTREEVIEWDRAGDROP_H

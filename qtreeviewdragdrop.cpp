#include "qtreeviewdragdrop.h"

QTreeViewDragDrop::QTreeViewDragDrop(QWidget *parent)
	: QTreeView(parent)
{

}

void QTreeViewDragDrop::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

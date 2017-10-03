#include "qlayoutwireitem.h"

QLayoutWireItem::QLayoutWireItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_dragged(false)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QLayoutWireItem::QLayoutWireItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QLayoutWireItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QLayoutWireItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void QLayoutWireItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}

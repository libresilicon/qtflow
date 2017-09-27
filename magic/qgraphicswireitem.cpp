#include "qgraphicswireitem.h"

QGraphicsWireItem::QGraphicsWireItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_dragged(false)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QGraphicsWireItem::QGraphicsWireItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QGraphicsWireItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QGraphicsWireItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void QGraphicsWireItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}

#include "qgraphicslayoutrectitem.h"

QGraphicsLayoutRectItem::QGraphicsLayoutRectItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_dragged(false)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QGraphicsLayoutRectItem::QGraphicsLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QGraphicsLayoutRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "move";
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
	scene()->update();
}

void QGraphicsLayoutRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void QGraphicsLayoutRectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}

#include "qgraphicslayoutrectitem.h"

QGraphicsLayoutRectItem::QGraphicsLayoutRectItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptDrops(true);
}

QGraphicsLayoutRectItem::QGraphicsLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent)
{

}

void QGraphicsLayoutRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "QGraphicsLayoutRectItem::" << __FUNCTION__;
	this->setPos(mapToScene(event->pos()));
}

void QGraphicsLayoutRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "QGraphicsLayoutRectItem::" << __FUNCTION__;
	this->setCursor(QCursor(Qt::ArrowCursor));
	Q_UNUSED(event);
}

void QGraphicsLayoutRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "QGraphicsLayoutRectItem::" << __FUNCTION__;
	this->setCursor(QCursor(Qt::ClosedHandCursor));
	Q_UNUSED(event);
}

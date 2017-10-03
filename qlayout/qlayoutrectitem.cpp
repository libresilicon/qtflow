#include "qlayoutrectitem.h"

QLayoutRectItem::QLayoutRectItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_locked(false)
{
	//setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
	//setFlags(QGraphicsItem::ItemIsMovable);
}

QLayoutRectItem::QLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_locked(false)
{
	//setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
	//setFlags(QGraphicsItem::ItemIsMovable);
}

bool QLayoutRectItem::contains(const QPointF &point) const
{
	QPointF offsetOrig;
	offsetOrig.setX(point.x()-x());
	offsetOrig.setY(point.y()-y());
	return QGraphicsRectItem::contains(offsetOrig);
}


qreal QLayoutRectItem::rectX() const
{
	return pos().x() + rect().x();
}

qreal QLayoutRectItem::rectY() const
{
	return pos().y() + rect().y();
}

qreal QLayoutRectItem::rectWidth() const
{
	return rect().width();
}

qreal QLayoutRectItem::rectHeight() const
{
	return rect().height();
}

bool QLayoutRectItem::isLocked()
{
	return m_locked;
}

void QLayoutRectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}

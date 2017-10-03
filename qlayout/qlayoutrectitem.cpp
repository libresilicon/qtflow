#include "qlayoutrectitem.h"

QLayoutRectItem::QLayoutRectItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent)
{
	//setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
	//setFlags(QGraphicsItem::ItemIsMovable);
}

QLayoutRectItem::QLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent)
{
	//setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
	//setFlags(QGraphicsItem::ItemIsMovable);
}


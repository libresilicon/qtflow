#include "qlayoutdistancemeasure.h"

QLayoutDistanceMeasure::QLayoutDistanceMeasure(QString label, QPointF start, QPointF end, QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
	m_mainLine = new QGraphicsLineItem(QLineF(start,end), this);
	m_label = new QGraphicsSimpleTextItem(label, m_mainLine);
}

QRectF QLayoutDistanceMeasure::boundingRect() const
{

}

void QLayoutDistanceMeasure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}


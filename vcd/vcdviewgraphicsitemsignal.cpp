#include "vcdviewgraphicsitemsignal.h"

VcdViewGraphicsItemSignal::VcdViewGraphicsItemSignal(QGraphicsItem *parent) :
	QGraphicsItem(parent)
{

}

QRectF VcdViewGraphicsItemSignal::boundingRect() const
{
	return QRect(m_xmin,m_ymin,m_xmax-m_xmin,m_ymax-m_ymin);
}

void VcdViewGraphicsItemSignal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void VcdViewGraphicsItemSignal::setBoundingRect()
{
}

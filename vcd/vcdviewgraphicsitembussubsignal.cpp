#include "vcdviewgraphicsitembussubsignal.h"

VcdViewGraphicsItemBusSubSignal::VcdViewGraphicsItemBusSubSignal(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
}

QRectF VcdViewGraphicsItemBusSubSignal::boundingRect() const
{
	return QRect(m_xmin,m_ymin,m_xmax-m_xmin,m_ymax-m_ymin);
}

void VcdViewGraphicsItemBusSubSignal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void VcdViewGraphicsItemBusSubSignal::setBoundingRect()
{
}

#include "vcdviewgraphicsitembus.h"

VcdViewGraphicsItemBus::VcdViewGraphicsItemBus(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
}

QRectF VcdViewGraphicsItemBus::boundingRect() const
{
	return QRect(m_xmin,m_ymin,m_xmax-m_xmin,m_ymax-m_ymin);
}

void VcdViewGraphicsItemBus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void VcdViewGraphicsItemBus::setBoundingRect()
{
}

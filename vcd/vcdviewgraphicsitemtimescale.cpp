#include "vcdviewgraphicsitemtimescale.h"

VcdViewGraphicsItemTimeScale::VcdViewGraphicsItemTimeScale(QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
}

QRectF VcdViewGraphicsItemTimeScale::boundingRect() const
{
	//return QRect(m_xmin,m_ymin,m_xmax-m_xmin,m_ymax-m_ymin);
}

void VcdViewGraphicsItemTimeScale::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}


#include "qschematicspart.h"

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, QString name, int x, int y)
{
	QGraphicsRectItem* r = obj->createRect(this);
	m_externalRect = r->rect();
	setPos(x,y);
}

void QSchematicsPart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(QPen(Qt::black));
	painter->drawPath(shape());
}

QPainterPath QSchematicsPart::shape() const
{
	QPainterPath path;
	path.addRect(m_externalRect);

	return path; // return the item's defined shape
}

QRectF QSchematicsPart::boundingRect() const
{
	return m_externalRect;
}

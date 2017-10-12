#include "qschematicspart.h"

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, QString name, int x, int y)
{
	m_name = name;
	QGraphicsRectItem* r = obj->createRect(this);
	m_externalRect = r->rect();
	obj;
	setPos(x,y);

	m_pins = obj->createPins(this);
}

void QSchematicsPart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(QPen(Qt::black));
	painter->drawPath(shape());
}

QRectF QSchematicsPart::boundingRect() const
{
	return m_externalRect;
}

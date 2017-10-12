#include "qschematicspart.h"

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, QString name, int x, int y)
{
	m_name = name;
	QGraphicsRectItem* r = obj->createRect(this);
	m_externalRect = r->rect();

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);
}

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, int x, int y)
{
	m_name = obj->getPrefix();
	QGraphicsRectItem* r = obj->createRect(this);
	m_externalRect = r->rect();

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(m_name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);
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

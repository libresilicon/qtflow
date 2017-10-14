#include "qschematicspart.h"

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, QString name, int x, int y, QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
	m_name = name;

	m_externalRect = obj->createRect(this);

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(m_name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);

	setBoundingRect();
}

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, int x, int y, QGraphicsItem *parent) :
	QGraphicsItem(parent)
{
	m_name = obj->getPrefix();

	m_externalRect = obj->createRect(this);

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(m_name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);

	setBoundingRect();
}

void QSchematicsPart::setBoundingRect()
{
	m_xmax = 0;
	m_ymax = 0;

	foreach(QSchematicsPin* p, m_pins) {
		if(p->x()>m_xmax) m_xmax = p->x();
		if(p->y()>m_ymax) m_ymax = p->y();
	}

	m_xmin = m_xmax;
	m_ymin = m_ymax;

	foreach(QSchematicsPin* p, m_pins) {
		if(p->x()<m_xmin) m_xmin = p->x();
		if(p->y()<m_ymin) m_ymin = p->y();
	}
}

QRectF QSchematicsPart::boundingRect() const
{
	return QRect(m_xmin,m_ymin,m_xmax-m_xmin,m_ymax-m_ymin);
}

void QSchematicsPart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

#include "schematicspart.h"

SchematicsPart::SchematicsPart()
{
}

void SchematicsPart::setType(QString n)
{
	m_type = n;
}

void SchematicsPart::setName(QString n)
{
	m_name = n;
}

void SchematicsPart::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

QString SchematicsPart::getName()
{
	return m_name;
}

QString SchematicsPart::getType()
{
	return m_type;
}

qreal SchematicsPart::x()
{
	return m_x;
}

qreal SchematicsPart::y()
{
	return m_y;
}

#include "schematicspart.h"

SchematicsPart::SchematicsPart(QString name, QString type)
{
	m_name = name;
	m_type = type;
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

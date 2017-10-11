#include "schematicspart.h"

SchematicsPart::SchematicsPart(QString type, QString id)
{
	m_type = type;
	m_id = id;
}

void SchematicsPart::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

QString SchematicsPart::getType()
{
	return m_type;
}

QString SchematicsPart::getID()
{
	return m_id;
}

qreal SchematicsPart::x()
{
	return m_x;
}

qreal SchematicsPart::y()
{
	return m_y;
}

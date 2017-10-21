#include "gdscell.h"

GDSCell::GDSCell(QString name):
	m_name(name)
{
}

QString GDSCell::getName()
{
	return m_name;
}


QVector<GDSBoundary*> GDSCell::getBoundaries()
{
	return m_boundaries;
}

void GDSCell::addBoundary(GDSBoundary* b)
{
	m_boundaries.append(b);
}

#include "gdscell.h"

GDSCell::GDSCell(QString name):
	m_name(name)
{
}

QString GDSCell::getName()
{
	return m_name;
}

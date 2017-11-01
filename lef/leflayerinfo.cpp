#include "leflayerinfo.h"

LEFLayerInfo::LEFLayerInfo() :
	m_pitch(0)
{
}

void LEFLayerInfo::setName(QString s)
{
	m_name = s;
}

void LEFLayerInfo::setType(QString s)
{
	m_type = s;
}

void LEFLayerInfo::setPitch(double i)
{
	m_pitch = i;
}

QString LEFLayerInfo::getName()
{
	return m_name;
}

QString LEFLayerInfo::getType()
{
	return m_type;
}

double LEFLayerInfo::getPitch()
{
	return m_pitch;
}


#include "defrouteinfo.h"

DEFRouteInfo::DEFRouteInfo()
{

}

void DEFRouteInfo::addPoint(qreal x, qreal y)
{
	m_pointList.append(QPointF(x,y));
}

void DEFRouteInfo::setLayer(QString s)
{
	m_layer = s;
}

QVector<QPointF> DEFRouteInfo::getPoints()
{
	return m_pointList;
}

QString DEFRouteInfo::getLayer()
{
	return m_layer;
}

void DEFRouteInfo::setVia(QString s)
{
	m_viaName = s;
}

QString DEFRouteInfo::getViaName()
{
	return m_viaName;
}

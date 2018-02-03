#include "gdsboundary.h"

GDSBoundary::GDSBoundary(int i) :
	m_layer(i),
	m_scale_x(1.0),
	m_scale_y(1.0),
	m_offset_x(0),
	m_offset_y(0)
{
}

void GDSBoundary::addPoint(int x, int y)
{
	GDSPoint* point = new GDSPoint();
	point->x = x;
	point->y = y;
	m_points.append(point);
}

qreal GDSBoundary::x()
{
	GDSPoint* p;
	qreal x;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	x = p->x;
	foreach(p, m_points) {
		if(x > p->x) x = p->x;
	}
	return x;
}

qreal GDSBoundary::y()
{
	GDSPoint* p;
	qreal y;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	y = p->y;
	foreach(p, m_points) {
		if(y > p->y) y = p->y;
	}
	return y;
}

qreal GDSBoundary::width()
{
	GDSPoint* p;
	qreal x1, x2;
	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	x2 = p->x;
	foreach(p, m_points) {
		if(x2 < p->x) x2 = p->x;
	}
	x1 = x2;
	foreach(p, m_points) {
		if(x1 > p->x) x1 = p->x;
	}
	return qFabs(x2-x1);
}

qreal GDSBoundary::height()
{
	GDSPoint* p;
	qreal y1, y2;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	y2 = p->y;
	foreach(p, m_points) {
		if(y2 < p->y) y2 = p->y;
	}
	y1 = y2;
	foreach(p, m_points) {
		if(y1 > p->y) y1 = p->y;
	}
	return y2-y1;
}

int GDSBoundary::getLayerIndex()
{
	return m_layer;
}

void GDSBoundary::setScale(qreal x, qreal y)
{
	m_scale_x = x;
	m_scale_y = y;
}

void GDSBoundary::setOffset(qreal x, qreal y)
{
	m_offset_x = x;
	m_offset_y = y;
}

QPolygonF GDSBoundary::getPolygon()
{
	QPolygonF polygon;
	qreal x, y;
	foreach(GDSPoint* p, m_points) {
		x = p->x;
		y = p->y;
		x/=m_scale_x;
		y/=m_scale_y;
		x += m_offset_x;
		y += m_offset_y;
		polygon << QPointF(x,y);
	}
	return polygon;
}

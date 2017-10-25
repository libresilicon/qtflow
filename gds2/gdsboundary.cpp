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

int GDSBoundary::x()
{
	GDSPoint* p;
	int x;
	int i = 0;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	x = p->x;
	foreach(p, m_points) {
		if(i) {
			if(x > p->x) x = p->x;
		}
		i++;
	}
	return x;
}

int GDSBoundary::y()
{
	GDSPoint* p;
	int y;
	int i = 0;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	y = p->y;
	foreach(p, m_points) {
		if(i) {
			if(y > p->y) y = p->y;
		}
		i++;
	}
	return y;
}

int GDSBoundary::width()
{
	GDSPoint* p;
	int x;
	int i = 0;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	x = p->x;
	foreach(p, m_points) {
		if(i) {
			if(x < p->x) x = p->x;
		}
		i++;
	}
	return x-this->x();
}

int GDSBoundary::height()
{
	GDSPoint* p;
	int y;
	int i = 0;

	if(m_points.count()==0) return 0;

	p = m_points.at(0);
	y = p->y;
	foreach(p, m_points) {
		if(i) {
			if(y < p->y) y = p->y;
		}
		i++;
	}
	return y-this->y();
}

int GDSBoundary::getLayerIndex()
{
	return m_layer;
}

QVector<QPointF> GDSBoundary::sortPoints()
{
	int x, y;

	QVector<QPointF> pl;

	int i = 0;
	foreach(GDSPoint* p, m_points) {
		if(i) {
			x = p->x;
			y = p->y;

			x/=m_scale_x;
			y/=m_scale_y;

			x += m_offset_x;
			y += m_offset_y;

			pl.append(QPointF(x,y));
		}
		i++;
	}

	return pl;
}

void GDSBoundary::setScale(int x, int y)
{
	m_scale_x = x;
	m_scale_y = y;
}

void GDSBoundary::setOffset(int x, int y)
{
	m_offset_x = x;
	m_offset_y = y;
}

void GDSBoundary::fillInPoints(QGraphicsPolygonItem *o)
{
	QPolygonF polygon;
	foreach(QPointF p, sortPoints()) {
		polygon << p;
	}
	o->setPolygon(polygon);
}

#include "gdsboundary.h"

GDSBoundary::GDSBoundary(int i) :
	m_layer(i)
{
}

void GDSBoundary::addPoint(int x, int y)
{
	GDSPoint* point = new GDSPoint();
	point->x = x;
	point->y = y;
	m_points.append(point);
}

int GDSBoundary::getLayerIndex()
{
	return m_layer;
}

QVector<QPointF> GDSBoundary::sortPoints()
{
	int m_scale = 100;
	GDSPoint* p0;
	int x0, y0;
	int x, y;

	QVector<QPointF> pl;

	p0 = m_points.at(0);
	x0 = p0->x;
	y0 = p0->y;

	int i = 0;
	foreach(GDSPoint* p, m_points) {
		if(i) {
			x = p->x;
			y = p->y;
			//x-= x0;
			//y-= y0;
			x/=m_scale;
			y/=m_scale;
			pl.append(QPointF(x,y));
		}
		i++;
	}

	return pl;
}

void GDSBoundary::fillInPoints(QGraphicsPolygonItem *o)
{
	QPolygonF polygon;
	foreach(QPointF p, sortPoints()) {
		polygon << p;
		qDebug() << p;
	}
	o->setPolygon(polygon);
}

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

void GDSBoundary::fillInPoints(QGraphicsPolygonItem *o)
{
	QPolygonF polygon;
	int i=0;
	foreach(GDSPoint* p, m_points) {
		polygon << QPointF(p->x/10,p->y/10);
		if(i==1) break;
		i++;
	}
	//o->setPolygon(polygon);
}

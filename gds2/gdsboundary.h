#ifndef GDSBOUNDARY_H
#define GDSBOUNDARY_H

#include <QGraphicsPolygonItem>

#include "gdspoint.h"

class GDSBoundary
{
public:
	GDSBoundary(int i);
	void addPoint(int x, int y);
	int getLayerIndex();
	void fillInPoints(QGraphicsPolygonItem* o);

private:
	QVector<GDSPoint*> m_points;
	int m_layer;
};

#endif // GDSBOUNDARY_H

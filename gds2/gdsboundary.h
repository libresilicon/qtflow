#ifndef GDSBOUNDARY_H
#define GDSBOUNDARY_H

#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QDebug>

#include "gdspoint.h"

class GDSBoundary
{
public:
	GDSBoundary(int i);
	void addPoint(int x, int y);
	int getLayerIndex();
	void fillInPoints(QGraphicsPolygonItem* o);

	void setScale(int x, int y);
	void setOffset(int x, int y);

	int x();
	int y();
	int width();
	int height();

private:
	QVector<QPointF> sortPoints();

	QVector<GDSPoint*> m_points;
	int m_layer;

	qreal m_scale_x;
	qreal m_scale_y;

	int m_offset_x;
	int m_offset_y;
};

#endif // GDSBOUNDARY_H

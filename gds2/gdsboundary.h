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
	QPolygonF getPolygon();

	void setScale(qreal x, qreal y);
	void setOffset(qreal x, qreal y);

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

	qreal m_offset_x;
	qreal m_offset_y;
};

#endif // GDSBOUNDARY_H

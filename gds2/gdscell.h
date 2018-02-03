#ifndef GDSCELL_H
#define GDSCELL_H

#include <QString>
#include <QPointF>
#include <QVector>
#include <QtMath>

#include "gdsboundary.h"

#include  "qlayout/qlayoutmacroitem.h"

typedef QVector<QPointF> path_t;

class GDSCell
{
public:
	GDSCell(QString name);
	void addBoundary(GDSBoundary* b);
	QString getName();
	QVector<GDSBoundary*> getBoundaries();

	qreal getWidth();
	qreal getHeight();

	void setRectangle(qreal x, qreal y, qreal w, qreal h);

private:
	QVector<GDSBoundary*> m_boundaries;
	QString m_name;
	qreal m_x1;
	qreal m_y1;
	qreal m_x2;
	qreal m_y2;

	qreal m_scale;

	qreal m_offset_x;
	qreal m_offset_y;
};

#endif // GDSCELL_H

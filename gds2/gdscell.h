#ifndef GDSCELL_H
#define GDSCELL_H

#include <QString>
#include <QPointF>
#include <QVector>

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

	void setRectangle(int x, int y, int w, int h);

private:
	QVector<GDSBoundary*> m_boundaries;
	QString m_name;
	int m_x;
	int m_y;
	int m_width;
	int m_height;

	qreal m_scale_x;
	qreal m_scale_y;

	int m_offset_x;
	int m_offset_y;
};

#endif // GDSCELL_H

#ifndef GDSCELL_H
#define GDSCELL_H

#include <QString>
#include <QPointF>
#include <QVector>

#include "gdsboundary.h"

typedef QVector<QPointF> path_t;

class GDSCell
{
public:
	GDSCell(QString name);
	void addBoundary(GDSBoundary* b);
	QString getName();
	QVector<GDSBoundary*> getBoundaries();

private:
	QVector<GDSBoundary*> m_boundaries;
	QString m_name;
};

#endif // GDSCELL_H

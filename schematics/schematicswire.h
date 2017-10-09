#ifndef SCHEMATICSWIRE_H
#define SCHEMATICSWIRE_H

#include <QString>
#include <QPointF>

class SchematicsWire
{
public:
	SchematicsWire();
	SchematicsWire(QString n, int a, int b, int c, int d);

	QString getName();
	QPointF getPos1();
	QPointF getPos2();

private:
	QString name;
	qreal x1, x2, y1, y2;
};

#endif // SCHEMATICSWIRE_H

#include "schematicswire.h"

SchematicsWire::SchematicsWire() :
	name(QString()),
	x1(0),
	y1(0),
	x2(0),
	y2(0)
{
}

SchematicsWire::SchematicsWire(QString n, int a, int b, int c, int d)
{
	name = n;
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;
}

QString SchematicsWire::getName()
{
	return name;
}

QPointF SchematicsWire::getP1()
{
	return QPointF(x1,y1);
}

QPointF SchematicsWire::getP2()
{
	return QPointF(x2,y2);
}

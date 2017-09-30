#ifndef COLORMAP_H
#define COLORMAP_H

#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>

class CMColor
{
public:
	CMColor();
	int r;
	int g;
	int b;
	QString name;
	int index;
};

class StippleSpec
{
public:
	StippleSpec();
	int index;
	unsigned int values[8];
};

class LayoutStyleSpec
{
public:
	LayoutStyleSpec();
	int num;
	int mask;
	int color;
	int outline;
	QString fill;
	int stipple_number;
	QString short_name;
	QString long_name;
};

class ColorMap
{
public:
	ColorMap();
	QColor colorFromName(QString s);
	bool isStipple(QString s);

	void loadColors(QString s);
	void loadDesign(QString s);

private:
	QColor colorFromCode(int i);

	QVector<CMColor> colorMap;
	QVector<LayoutStyleSpec> layoutStyleMap;
	QVector<LayoutStyleSpec> paleStyleMap;
	QVector<LayoutStyleSpec> displayStyleMap;
	QVector<StippleSpec> stippleMap;
};

#endif // COLORMAP_H

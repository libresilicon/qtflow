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
	QColor color;
	QString name;
	int index;
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
	QColor fromName(QString s);
	void loadColors(QString s);
	void loadDesign(QString s);

private:
	QColor fromCode(int i);

	QVector<CMColor> colorMap;
	QVector<LayoutStyleSpec> layoutStyleMap;
};

#endif // COLORMAP_H

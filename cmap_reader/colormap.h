#ifndef COLORMAP_H
#define COLORMAP_H

#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QImage>
#include <QPixmap>
#include <QByteArray>

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
	QByteArray values;
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
	bool colorNameExists(QString s);
	QColor colorFromCode(int i);
	QString colorNameFromCode(int i);
	QString materialNameFromCode(int i);
	bool isStipple(QString s);
	int getStippleID(QString s);
	QPixmap getStipplePixMap(QString s);

	void loadColors(QString s);
	void loadDesign(QString s);

private:
	QVector<CMColor> colorMap;
	QVector<LayoutStyleSpec> layoutStyleMap;
	QVector<LayoutStyleSpec> paleStyleMap;
	QVector<LayoutStyleSpec> displayStyleMap;
	QVector<StippleSpec> stippleMap;
};

#endif // COLORMAP_H

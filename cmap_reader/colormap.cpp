#include "colormap.h"

CMColor::CMColor()
{}

LayoutStyleSpec::LayoutStyleSpec()
{}

StippleSpec::StippleSpec()
{}

ColorMap::ColorMap()
{}

void ColorMap::loadColors(QString s)
{
	CMColor color;
	QString line;
	QStringList list;
	QFile input(s);
	if (input.open(QIODevice::ReadOnly))
	{
		QTextStream in(&input);
		while (!in.atEnd())
		{
			line = in.readLine();
			list = line.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(list.count()>0) {
				if(!list[0].contains("#")) {
					if(list.count()>3) {
						color = CMColor();
						color.index = list[3].toInt();
						color.r = list[0].toInt();
						color.g =list[1].toInt();
						color.b = list[2].toInt();
						if(list.count()>4) {
							color.name = list[4];
						}
						colorMap.append(color);
					}
				}
			}
		}
		input.close();
	}
}

enum load_design_mode {
	display_styles,
	layout_styles,
	pale_styles,
	stipples
};

void ColorMap::loadDesign(QString s)
{
	LayoutStyleSpec style;
	StippleSpec stipple;
	load_design_mode recentMode;
	QString line;
	QStringList list;
	QString hexvalue;
	bool ok;
	uchar stippleValue;

	QFile input(s);	
	if (input.open(QIODevice::ReadOnly))
	{
		QTextStream in(&input);
		while (!in.atEnd())
		{
			line = in.readLine();
			list = line.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(list.count()>0) {
				if(!list[0].contains("#")) {
					if(list[0]=="display_styles") recentMode = display_styles;
					else if(list[0]=="layout_styles") recentMode = layout_styles;
					else if(list[0]=="pale_styles") recentMode = pale_styles;
					else if(list[0]=="stipples") recentMode = stipples;
					else {
						switch(recentMode) {
							case display_styles:
								if(list.count()>7) {
									style = LayoutStyleSpec();
									style.num = list[0].toInt();
									style.mask = list[1].toInt();
									style.color = list[2].toInt();
									style.outline = list[3].toInt();
									style.fill = list[4];
									style.stipple_number = list[5].toInt();
									style.short_name = list[6];
									style.long_name = list[7];
									displayStyleMap.append(style);
								}
								break;

							case pale_styles:
								if(list.count()>7) {
									style = LayoutStyleSpec();
									style.num = list[0].toInt();
									style.mask = list[1].toInt();
									style.color = list[2].toInt();
									style.outline = list[3].toInt();
									style.fill = list[4];
									style.stipple_number = list[5].toInt();
									style.short_name = list[6];
									style.long_name = list[7];
									paleStyleMap.append(style);
								}
								break;

							case layout_styles:
								if(list.count()>7) {
									style = LayoutStyleSpec();
									style.num = list[0].toInt();
									style.mask = list[1].toInt();
									style.color = list[2].toInt();
									style.outline = list[3].toInt();
									style.fill = list[4];
									style.stipple_number = list[5].toInt();
									style.short_name = list[6];
									style.long_name = list[7];
									layoutStyleMap.append(style);
								}
								break;

							case stipples:
								if(list.count()>8) {
									stipple = StippleSpec();
									stipple.index = list[0].toInt();
									for(int i=1; i<9; i++) {
										hexvalue = "0x"+list[i];
										stipple.values[i-1] = (char)hexvalue.toUInt(&ok,16);
										if(!ok) break;
									}
									if(!ok) break;
									stippleMap.append(stipple);
								}
								break;
						}
					}
				}
			}
		}
		input.close();
	}
}

QColor ColorMap::colorFromCode(int i)
{
	foreach(CMColor color, colorMap) {
		if(color.index==i) {
			return QColor(color.r,color.g,color.b);
		}
	}
	qDebug() << "Color " << i << " not defined";
	return QColor(Qt::green);
}


QColor ColorMap::colorFromName(QString s)
{
	LayoutStyleSpec spec;

	foreach(spec, layoutStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			return colorFromCode(spec.color);
		}
	}

	foreach(spec, paleStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			return colorFromCode(spec.color);
		}
	}

	foreach(spec, displayStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			return colorFromCode(spec.color);
		}
	}

	return QColor(Qt::red);
}

bool ColorMap::isStipple(QString s)
{
	foreach(LayoutStyleSpec spec, layoutStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			if(spec.fill.contains("stipple"))
				return true;
		}
	}
	return false;
}

int ColorMap::getStippleID(QString s)
{
	int ret = 0;
	foreach(LayoutStyleSpec spec, layoutStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			ret = spec.stipple_number;
		}
	}
	return ret;
}

QPixmap ColorMap::getStipplePixMap(QString s)
{
	QImage img;
	QPixmap pm;
	uchar pixData[8];


	foreach(StippleSpec stipple, stippleMap) {
		if(stipple.index==getStippleID(s)) {
			for(int i=0; i<8; i++) {
				pixData[i]=stipple.values[i];
			}
			break;
		}
	}

	img = QImage(pixData, 1, 1, 8, QImage::Format_RGB32);
	pm = QPixmap::fromImage(img.scaled(100, 100));
	//if(pm.loadFromData(pixData,Qt::AutoColor))
	//	qDebug() << "Loaded successfully";

	//pm.scaled(200,200);

	return pm;
}

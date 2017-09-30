#include "colormap.h"

CMColor::CMColor()
{}

LayoutStyleSpec::LayoutStyleSpec()
{}

ColorMap::ColorMap()
{
}

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
						color.color = QColor(list[0].toInt(),list[1].toInt(),list[2].toInt());
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
	load_design_mode recentMode;
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
					if(list[0]=="display_styles") recentMode = display_styles;
					else if(list[0]=="layout_styles") recentMode = layout_styles;
					else if(list[0]=="pale_styles") recentMode = pale_styles;
					else if(list[0]=="stipples") recentMode = stipples;
					else {
						switch(recentMode) {
							case display_styles:
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

							case pale_styles:
								break;

							case stipples:
								break;
						}
					}
				}
			}
		}
		input.close();
	}
}

QColor ColorMap::fromCode(int i)
{
	foreach(CMColor color, colorMap) {
		if(color.index==i) return color.color;
	}
	return QColor(Qt::black);
}


QColor ColorMap::fromName(QString s)
{
	foreach(LayoutStyleSpec spec, layoutStyleMap) {
		if((spec.long_name==s)||(spec.short_name==s)) {
			return fromCode(spec.color);
		}
	}
	return QColor(Qt::black);
}

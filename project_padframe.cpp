#include "project.h"

void Project::buildPadFrame()
{
	lef::LEFMacro* m;
	lef::LEFPin* p;
	qreal height_corner2;
	qreal height_corner4;
	int mx,my;
	qreal mw,mh;
	qreal coreleft,coreright,corebottom,coretop; // die dimensions
	qreal corew, coreh;
	qreal padframew,padframeh; // pad frame dimensions
	qreal len;
	QStringList sides;
	sides << "T" << "B" << "L" << "R";

	QMap<QString,int> cellCounters;
	QString pl1path = QDir(getLayoutDir()).filePath(getTopLevel()+".pl1");
	QString pinpath = QDir(getLayoutDir()).filePath(getTopLevel()+".pin");
	QString padspath = getPadInfoFile();

	QFile pl1file(pl1path);
	QFile pinfile(pinpath);

	QString padName;
	QString cellName;
	QPointF pinCenter;
	QString pinSignal;
	QMap<QString,qreal> xoffsets;
	QMap<QString,qreal> yoffsets;
	QStringList lineList;
	QMap<QString,QString> componentList;

	int cx1,cy1,cx2,cy2;
	bool firstLine = true;

	if(pl1file.open(QIODevice::ReadOnly)) {
		QTextStream pl1stream(&pl1file);
		while (!pl1stream.atEnd()) {
			lineList = pl1stream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(lineList.count()>4) {
				cx1 = lineList[1].toInt();
				cy1 = lineList[2].toInt();
				cx2 = lineList[3].toInt();
				cy2 = lineList[4].toInt();
				if(firstLine) {
					coreleft=cx1;
					coreright=cx2;
					corebottom=cy1;
					coretop=cy2;
				} else {
					if(coreleft>cx1) coreleft=cx1;
					if(coreright<cx2) coreright=cx2;
					if(corebottom>cy1) corebottom=cy1;
					if(coretop<cy2) coretop=cy2;
				}
			}
		}
		pl1file.close();
	} else {
		qDebug() << pl1path << " can't be opened";
		return;
	}

	corew = coreright-coreleft;
	coreh = coretop-corebottom;

	foreach(cellName, getIOCells()) {
		cellCounters[cellName]=0;
	}

	if(QFileInfo(padspath).exists()) {
		PadInfo padInfo(padspath);

		padframew = 0;
		padframeh = 0;
		foreach(QString side, sides) {
			len = 0;
			for(int i=0;i<padInfo.getSideLength();i++) {
				padName=side+QString::number(i+1);
				cellName = padInfo.getPadCell(padName);
				m = getMacro(cellName);
				mw = 10;
				mh = 10;
				if(m) {
					mw = m->getWidth();
					mh = m->getHeight();
				}
				mw *= 100;
				mh *= 100;
				len+=mw;
			}
			if(side=="L") {
				m = getMacro(padInfo.getPadCell("CORNER1"));
				if(m) len+=m->getHeight()*100;
				m = getMacro(padInfo.getPadCell("CORNER2"));
				if(m) len+=m->getHeight()*100;
			}
			if(side=="R") {
				m = getMacro(padInfo.getPadCell("CORNER3"));
				if(m) len+=m->getHeight()*100;
				m = getMacro(padInfo.getPadCell("CORNER4"));
				if(m) len+=m->getHeight()*100;
			}
			if(side=="T") {
				m = getMacro(padInfo.getPadCell("CORNER1"));
				if(m) len+=m->getWidth()*100;
				m = getMacro(padInfo.getPadCell("CORNER3"));
				if(m) len+=m->getWidth()*100;
			}
			if(side=="B") {
				m = getMacro(padInfo.getPadCell("CORNER2"));
				if(m) len+=m->getWidth()*100;
				m = getMacro(padInfo.getPadCell("CORNER4"));
				if(m) len+=m->getWidth()*100;
			}
			if((side=="L")||(side=="R")) if(padframeh<len) padframeh=len;
			if((side=="T")||(side=="B")) if(padframew<len) padframew=len;
		}

		if(pl1file.open(QIODevice::WriteOnly|QIODevice::Append)) {
			QTextStream pl1stream(&pl1file);
			foreach(QString side, sides) {
				mx = (side=="R")?(coreleft+(corew/2))+(padframew/2):(coreleft+(corew/2))-(padframew/2);
				my = (side=="T")?(corebottom+(coreh/2))+(padframeh/2):(corebottom+(coreh/2))-(padframeh/2);
				if(side=="T") {
					cellName = padInfo.getPadCell("CORNER1");
					m = getMacro(cellName);
					mw = (m)?m->getWidth():100;
					mw *= 100;
					mh = (m)?m->getHeight():100;
					mh *= 100;
					my -= mh;

					pl1stream << cellName << "_1";
					pl1stream << " ";
					pl1stream << QString::number(mx) << " " << QString::number(my) << " ";
					pl1stream << QString::number(mx+mw) << " " << QString::number(my+mh) << " ";
					pl1stream << " ";
					pl1stream << "0";
					pl1stream << " ";
					pl1stream << "1";
					pl1stream << endl;
					cellCounters[cellName]++;
					mx+=mw;
				}
				if(side=="B") {
					cellName = padInfo.getPadCell("CORNER2");
					m = getMacro(cellName);
					mw = (m)?m->getWidth():100;
					mw *= 100;
					height_corner2 = (m)?m->getWidth():100;
					height_corner2 *= 100;

					pl1stream << cellName << "_2";
					pl1stream << " ";
					pl1stream << QString::number(mx) << " " << QString::number(my) << " ";
					pl1stream << QString::number(mx+mw) << " " << QString::number(my+mh) << " ";
					pl1stream << " ";
					pl1stream << "6";
					pl1stream << " ";
					pl1stream << "1";
					pl1stream << endl;
					cellCounters[cellName]++;
					mx+=mw;
				}
				if(side=="L") my+=height_corner2;
				if(side=="R") my+=height_corner4;
				if(side=="R") mx-=height_corner4;

				for(int i=0;i<padInfo.getSideLength();i++) {
					padName=side+QString::number(i+1);
					cellName = padInfo.getPadCell(padName);
					componentList[padName]=cellName;
					m = getMacro(cellName);
					mw = (m)?m->getWidth():100;
					mh = (m)?m->getHeight():100;
					mw *= 100;
					mh *= 100;

					pl1stream << cellName << "_" << QString::number(cellCounters[cellName]+1);
					pl1stream << " ";

					pl1stream << QString::number(mx) << " " << QString::number(my) << " ";
					pl1stream << QString::number(mx+mw) << " " << QString::number(my+mh) << " ";

					pl1stream << " ";
					if(side=="T") {
						pl1stream << "0";
					}
					if(side=="R") {
						pl1stream << "7";
					}
					if(side=="L") {
						pl1stream << "6";
					}
					if(side=="B") {
						pl1stream << "3";
					}
					pl1stream << " 1";
					pl1stream << endl;

					cellCounters[cellName]++;
					mx+=((side=="T")||(side=="B"))?mw:0;
					my+=((side=="R")||(side=="L"))?mw:0;
				}
				if(side=="T") {
					cellName = padInfo.getPadCell("CORNER3");
					m = getMacro(cellName);
					mw = (m)?m->getWidth():100;
					mw *= 100;

					pl1stream << cellName << "_3";
					pl1stream << " ";
					pl1stream << QString::number(mx) << " " << QString::number(my) << " ";
					pl1stream << QString::number(mx+mw) << " " << QString::number(my+mh) << " ";
					pl1stream << " ";
					pl1stream << "7";
					pl1stream << " ";
					pl1stream << "1";
					pl1stream << endl;
					cellCounters[cellName]++;
					mx+=mw;
				}
				if(side=="B") {
					cellName = padInfo.getPadCell("CORNER4");
					m = getMacro(cellName);
					mw = (m)?m->getWidth():100;
					mw *= 100;
					height_corner4 = (m)?m->getHeight():100;
					height_corner4 *= 100;

					pl1stream << cellName << "_4";
					pl1stream << " ";
					pl1stream << QString::number(mx) << " " << QString::number(my) << " ";
					pl1stream << QString::number(mx+mw) << " " << QString::number(my+mh) << " ";
					pl1stream << " ";
					pl1stream << "3";
					pl1stream << " ";
					pl1stream << "1";
					pl1stream << endl;
					cellCounters[cellName]++;
					mx+=mw;
				}

			}
			pl1file.close();
		} else {
			qDebug() << pl1path << " can't be opened";
			return;
		}

		foreach(cellName, getIOCells()) {
			cellCounters[cellName]=0;
		}

		/*int j = 0;
		if(pinfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
			QTextStream pinstream(&pinfile);
			foreach(QString side, sides) {
				for(int i=0;i<padInfo.getSideLength();i++) {
					padName=side+QString::number(i+1);
					cellName=padInfo.getPadCell(padName);
					m = getMacro(componentList[padName]);
					if(m) {
						foreach(QString pin, m->getPinNames()) {
							p = m->getPin(pin);
							pinSignal = padInfo.getPadPinSignal(padName,pin);
							if(pinSignal=="None") continue; // place holder

							pinstream << pinSignal;
							pinstream << " ";
							pinstream << j;
							pinstream << " ";
							pinstream << cellName;
							pinstream << "_";
							pinstream << cellCounters[cellName];
							pinstream << " ";
							pinstream << pin;
							pinstream << " ";
							if(p) {
								pinCenter = p->getCenter();
								pinstream << pinCenter.x();
								pinstream << " ";
								pinstream << pinCenter.y();
							} else {
								pinstream << 0;
								pinstream << " ";
								pinstream << 0;
							}

							pinstream << " ";
							if(side=="T") {
								pinstream << "0";
							}
							if(side=="R") {
								pinstream << "7";
							}
							if(side=="L") {
								pinstream << "6";
							}
							if(side=="B") {
								pinstream << "3";
							}
							pinstream << " 1 1";
							pinstream << endl;

							cellCounters[cellName]++;
							j++;
						}
					}
				}
			}
			pinfile.close();
		} else {
			qDebug() << pinpath << " can't be opened";
			return;
		}*/
	}
}

QString Project::getPadInfoFile()
{
	return QDir(getSourceDir()).filePath(getTopLevel()+".pads");
}

QString Project::getPadFrameFile()
{
	return QDir(getLayoutDir()).filePath("padframe.mag");
}

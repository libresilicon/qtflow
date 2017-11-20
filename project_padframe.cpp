#include "project.h"

void Project::buildPadFrame()
{
	lef::LEFMacro* m;
	lef::LEFPin* p;
	qreal height_corner = 0;
	int mx,my;
	int j;
	qreal mw,mh;
	qreal px,py;
	qreal pw,ph;
	qreal coreleft,coreright,corebottom,coretop; // die dimensions
	qreal padframeleft,padframeright,padframebottom,padframetop; // padframe dimensions
	qreal corew, coreh;
	qreal padframew,padframeh; // pad frame dimensions
	qreal len;
	QStringList sides;
	sides << "T" << "B" << "L" << "R";

	QMap<QString,QString> indexedCellInstance;
	QString pl1path = QDir(getLayoutDir()).filePath(getTopLevel()+".pl1");
	QString pl2path = QDir(getLayoutDir()).filePath(getTopLevel()+".pl2");
	QString pinpath = QDir(getLayoutDir()).filePath(getTopLevel()+".pin");
	QString preroutepath = QDir(getLayoutDir()).filePath(getTopLevel()+".preroute");
	QString padspath = getPadInfoFile();

	QFile pl1file(pl1path);
	QFile pl2file(pl2path);
	QFile pinfile(pinpath);
	QFile preroutefile(preroutepath);

	QString padName;
	QString cellName;
	QPointF pinCenter;
	QString pinSignal;
	QStringList lineList;
	QMap<QString,int> instanceOrientationMapping;
	QMap<QString,QRectF> instanceBox;
	int orient;

	pw = 100;
	ph = 100;

	if(pl1file.open(QIODevice::ReadOnly)) {
		bool firstLine = true;
		int cx1,cy1,cx2,cy2;
		QTextStream pl1stream(&pl1file);
		firstLine = true;
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
					firstLine = false;
				} else {
					if(cx1<coreleft) coreleft=cx1;
					if(cx2<coreleft) coreleft=cx2;
					if(coreright<cx1) coreright=cx1;
					if(coreright<cx2) coreright=cx2;
					if(cy1<corebottom) corebottom=cy1;
					if(cy2<corebottom) corebottom=cy2;
					if(coretop<cy1) coretop=cy1;
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

	if(QFileInfo(padspath).exists()) {
		PadInfo padInfo(padspath);

		instanceOrientationMapping["CORNER1"]=0; // left upper corner
		instanceOrientationMapping["CORNER2"]=6; // left lower corner
		instanceOrientationMapping["CORNER3"]=7; // right upper corner
		instanceOrientationMapping["CORNER4"]=3; // right lower corner
		foreach(QString side, sides) {
			for(int i=0;i<padInfo.getSideLength();i++) {
				padName=side+QString::number(i+1);
				if(side=="T") instanceOrientationMapping[padName]=0; // North
				if(side=="L") instanceOrientationMapping[padName]=6;
				if(side=="R") instanceOrientationMapping[padName]=7;
				if(side=="B") instanceOrientationMapping[padName]=3; // South
			}
		}

		padframew = 0;
		padframeh = 0;
		foreach(QString side, sides) {
			len = 0;
			for(int i=0;i<padInfo.getSideLength();i++) {
				padName=side+QString::number(i+1);
				cellName = padInfo.getPadCell(padName);
				m = getMacro(cellName);
				mw = ((m)?m->getWidth():100)*100; // next to each other
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

		padframeleft = (coreleft+(corew/2))-(padframew/2);
		padframeright = (coreleft+(corew/2))+(padframew/2);
		padframebottom = (corebottom+(coreh/2))-(padframeh/2);
		padframetop = (corebottom+(coreh/2))+(padframeh/2);

		cellName = padInfo.getPadCell("CORNER1");  // left upper corner
		m = getMacro(cellName);
		mw = ((m)?m->getWidth():100)*100;
		mh = ((m)?m->getHeight():100)*100;
		if(mh>height_corner) height_corner = mh;
		instanceBox["CORNER1"]=QRectF(padframeleft,padframetop-mh,mw,mh);

		cellName = padInfo.getPadCell("CORNER2"); // left lower corner
		m = getMacro(cellName);
		mw = ((m)?m->getWidth():100)*100;
		mh = ((m)?m->getHeight():100)*100;
		if(mh>height_corner) height_corner = mh;
		instanceBox["CORNER2"]=QRectF(padframeleft,padframebottom,mw,mh);

		cellName = padInfo.getPadCell("CORNER3"); // right upper corner
		m = getMacro(cellName);
		mw = ((m)?m->getWidth():100)*100;
		mh = ((m)?m->getHeight():100)*100;
		if(mh>height_corner) height_corner = mh;
		instanceBox["CORNER3"]=QRectF(padframeright-mw,padframetop-mh,mw,mh);

		cellName = padInfo.getPadCell("CORNER4"); // right lower corner
		m = getMacro(cellName);
		mw = ((m)?m->getWidth():100)*100;
		mh = ((m)?m->getHeight():100)*100;
		if(mh>height_corner) height_corner = mh;
		instanceBox["CORNER4"]=QRectF(padframeright-mw,padframebottom,mw,mh);

		foreach(QString side, sides) {
			mx = (side=="R")?padframeright-height_corner:padframeleft;
			my = (side=="T")?padframetop-height_corner:padframebottom;
			if((side=="R")||(side=="L")) my+=height_corner;
			if((side=="T")||(side=="B")) mx+=height_corner;
			for(int i=0;i<padInfo.getSideLength();i++) {
				padName=side+QString::number(i+1);
				cellName = padInfo.getPadCell(padName);
				m = getMacro(cellName);
				mw = ((m)?m->getWidth():100)*100;
				mh = ((m)?m->getHeight():100)*100;
				instanceBox[padName]=QRectF(mx,my,mw,mh);
				mx+=((side=="T")||(side=="B"))?mw:0;
				my+=((side=="R")||(side=="L"))?mw:0;
			}
		}

		QMap<QString,int> cellCounters;
		foreach(QString padName, instanceBox.keys()) {
			cellName = padInfo.getPadCell(padName);
			if(!cellCounters.contains(cellName)) cellCounters[cellName]=0;
			indexedCellInstance[padName]=cellName+"_"+QString::number(cellCounters[cellName]+1);
			cellCounters[cellName]++;
		}

		if(pl1file.open(QIODevice::WriteOnly|QIODevice::Append)) {
			QTextStream pl1stream(&pl1file);
			foreach(QString padName, instanceBox.keys()) {
				pl1stream << indexedCellInstance[padName];
				pl1stream << " ";
				pl1stream << QString::number(instanceBox[padName].x());
				pl1stream << " ";
				pl1stream << QString::number(instanceBox[padName].y());
				pl1stream << " ";
				pl1stream << QString::number(instanceBox[padName].x()+instanceBox[padName].width());
				pl1stream << " ";
				pl1stream << QString::number(instanceBox[padName].y()+instanceBox[padName].height());
				pl1stream << " ";
				pl1stream << QString::number(instanceOrientationMapping[padName]);
				pl1stream << " 1";
				pl1stream << endl;
			}
			pl1file.close();
		} else {
			qDebug() << pl1path << " can't be opened";
			return;
		}

		QMap<QString,QMap<QString,QMap<QString,QPointF>>> signalMacroPinPosition;
		QMap<QString,QMap<QString,QMap<QString,QPointF>>> signalMacroDeadPinPosition;

		qreal x,y;
		foreach(QString padName, instanceBox.keys()) {
			cellName = padInfo.getPadCell(padName);
			m = getMacro(cellName);
			if(m) {
				foreach(QString pin, m->getPinNames()) {
					p = m->getPin(pin);
					if(p) {
						pinSignal = padInfo.getPadPinSignal(padName,pin);
						pinCenter = p->getCenter()*100;
						x = pinCenter.x();
						y = pinCenter.y();
						if(instanceOrientationMapping[padName]==3) { // South
							px=-x;
							py=-y;
						} else if(instanceOrientationMapping[padName]==7) { // East
							px=y;
							py=-x;
						} else if(instanceOrientationMapping[padName]==6) { // West
							px=-y;
							py=x;
						} else {
							px=x;
							py=y;
						}
						px+=instanceBox[padName].x();
						py+=instanceBox[padName].y();
						if((pinSignal=="None")||(pinSignal==QString())) {
							signalMacroDeadPinPosition[pinSignal][padName][pin]=QPointF(px,py);
						} else {
							signalMacroPinPosition[pinSignal][padName][pin]=QPointF(px,py);
						}
					}
				}
			}
		}

		QString twpinName;
		QMap<QString,QPointF> twpinPosition;
		QMap<QString,int> twpinOrientationMapping;
		QMap<QString,QString> twpinSignalMapping;
		QMap<QString,QString> prerouteNetTypeMapping;
		qreal cored,padframed;
		cored = (corew>coreh)?corew:coreh;
		padframed = (padframew>padframeh)?padframew:padframeh;

		foreach(QString signal, signalMacroPinPosition.keys()) {
			foreach(QString padName, signalMacroPinPosition[signal].keys()) {
				foreach(QString pin, signalMacroPinPosition[signal][padName].keys()) {
					orient = instanceOrientationMapping[padName];
					px = signalMacroPinPosition[signal][padName][pin].x();
					py = signalMacroPinPosition[signal][padName][pin].y();
					twpinName = "twpin_"+signal;
					twpinPosition[twpinName]=QPointF(px,py);
					twpinOrientationMapping[twpinName]=orient;
					twpinSignalMapping[twpinName]=signal;
				}
			}
		}

		QMap<QString,QMap<QString,QRectF>> preRoutePinRectangles;
		QString netName;

		foreach(QString padName, instanceBox.keys()) {
			cellName = padInfo.getPadCell(padName);
			m = getMacro(cellName);
			if(m) {
				foreach(QString pin, m->getPinNames()) {
					p = m->getPin(pin);
					if(p) { // can't place this thing if NULL
						netName = padInfo.getPadPinSignal(padName,pin);
						preRoutePinRectangles[cellName][pin]=p->getBoundingBox();
						if((netName!=QString())&&(netName!="None")) {
							prerouteNetTypeMapping[netName] = "SIGNAL";
							if(getPowerNets().contains(netName)) prerouteNetTypeMapping[netName] = "POWER";
							if(getGroundNets().contains(netName)) prerouteNetTypeMapping[netName] = "GROUND";
							if(getClockNets().contains(netName)) prerouteNetTypeMapping[netName] = "CLOCK";
						}
					}
				}
			}
		}

		if(pl2file.open(QIODevice::WriteOnly|QIODevice::Append)) {
			QTextStream pl2stream(&pl2file);
			foreach(QString twpinName, twpinPosition.keys()) {
				pl2stream << twpinName << " ";
				pl2stream << twpinPosition[twpinName].x()-pw/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].y()-ph/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].x()+pw/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].y()+ph/2;
				pl2stream << " ";
				pl2stream << QString::number(twpinOrientationMapping[twpinName]);
				pl2stream << " 0 0";
				pl2stream << endl;
			}

			int j = 0;
			foreach(QString twpinName, twpinPosition.keys()) {
				pl2stream << "pin" << QString::number(j) << " ";
				pl2stream << twpinPosition[twpinName].x()-pw/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].y()-ph/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].x()+pw/2;
				pl2stream << " ";
				pl2stream << twpinPosition[twpinName].y()+ph/2;
				pl2stream << " ";
				pl2stream << QString::number(twpinOrientationMapping[twpinName]);
				pl2stream << " 0 0";
				pl2stream << endl;
				j++;
			}

			/*j=0;
			foreach(QString signal, signalMacroDeadPinPosition.keys()) {
				foreach(QString padName	, signalMacroDeadPinPosition[signal].keys()) {
					foreach(QString pin	, signalMacroDeadPinPosition[signal][padName].keys()) {
						pl2stream << "deadpin" << QString::number(j) << " ";
						pl2stream << signalMacroDeadPinPosition[signal][padName][pin].x()-pw/2;
						pl2stream << " ";
						pl2stream << signalMacroDeadPinPosition[signal][padName][pin].y()-ph/2;
						pl2stream << " ";
						pl2stream << signalMacroDeadPinPosition[signal][padName][pin].x()+pw/2;
						pl2stream << " ";
						pl2stream << signalMacroDeadPinPosition[signal][padName][pin].y()+ph/2;
						pl2stream << " ";
						pl2stream << QString::number(instanceOrientationMapping[padName]);
						pl2stream << " 0 0";
						pl2stream << endl;
						j++;
					}
				}
			}*/

			pl2file.close();
		} else {
			qDebug() << pl1path << " can't be opened";
			return;
		}

		j = 0;
		if(pinfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
			QTextStream pinstream(&pinfile);
			foreach(QString signal, signalMacroPinPosition.keys()) {
				foreach(QString padName	, signalMacroPinPosition[signal].keys()) {
					foreach(QString pin	, signalMacroPinPosition[signal][padName].keys()) {
						pinstream << signal << " ";
						pinstream << j << " ";
						pinstream << indexedCellInstance[padName] << " ";
						pinstream << pin << " ";
						pinstream << signalMacroPinPosition[signal][padName][pin].x();
						pinstream << " ";
						pinstream << signalMacroPinPosition[signal][padName][pin].y();
						pinstream << " ";
						pinstream << QString::number(instanceOrientationMapping[padName]);
						pinstream << " 1 1";
						pinstream << endl;
						j++;
					}
				}
			}
			foreach(QString twpinName, twpinPosition.keys()) {
				pinstream << twpinSignalMapping[twpinName] << " ";
				pinstream << j << " ";
				pinstream << "twpin_" << twpinSignalMapping[twpinName] << " ";
				pinstream << twpinSignalMapping[twpinName] << " ";
				pinstream << twpinPosition[twpinName].x() << " ";
				pinstream << twpinPosition[twpinName].y() << " ";
				pinstream << QString::number(twpinOrientationMapping[padName]);
				pinstream << " 1 1";
				pinstream << endl;
				j++;
			}
			pinfile.close();
		} else {
			qDebug() << pinpath << " can't be opened";
			return;
		}

		if(preroutefile.open(QIODevice::WriteOnly)) {
			QTextStream preroutestream(&preroutefile);
			preroutestream << "core ";
			preroutestream << coreleft << " " << coreright << " " << corebottom << " " << coretop << endl;
			preroutestream << "padframe ";
			preroutestream << padframeleft << " " << padframeright << " " << padframebottom << " " << padframetop << endl;
			foreach(QString componentName, preRoutePinRectangles.keys()) { // saving relative positions so that we don't have to read LEF from python
				foreach(QString pinName, preRoutePinRectangles[componentName].keys()) {
					preroutestream << "position";
					preroutestream << " " << componentName;
					preroutestream << " " << pinName;
					preroutestream << " " << preRoutePinRectangles[componentName][pinName].x()*100;
					preroutestream << " " << preRoutePinRectangles[componentName][pinName].y()*100;
					preroutestream << " " << preRoutePinRectangles[componentName][pinName].width()*100;
					preroutestream << " " << preRoutePinRectangles[componentName][pinName].height()*100;
					preroutestream << endl;
				}
				m = getMacro(componentName);  // saving macro dimension so that we don't have to read LEF from python
				if(m) {
					preroutestream << "dimension";
					preroutestream << " " << componentName;
					preroutestream << " " << QString::number(m->getWidth()*100);
					preroutestream << " " << QString::number(m->getHeight()*100);
					preroutestream << endl;
				}
			}
			foreach(QString padCell, getPadCells()) {
				preroutestream << "padcell " << padCell << endl;
			}
			foreach(QString net, prerouteNetTypeMapping.keys()) {
				preroutestream << "net " << net << endl;
				preroutestream << "use " << net << " " << prerouteNetTypeMapping[net] << endl;
				preroutestream << "layer " << net << " " << getSpecialNetLayer(net) << endl;
			}
			preroutefile.close();
		} else {
			qDebug() << preroutepath << " can't be opened";
			return;
		}
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

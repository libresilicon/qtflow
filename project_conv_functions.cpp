#include "project.h"

void Project::schematics2blif(QString top)
{
}

void Project::blif2cel(QString top)
{
	QString blifpath;
	QMap<QString,int> macroCounters;
	QString celpath;
	QString compname;
	QString celfileline;
	QString pinname;
	qreal mult;
	qreal w,h;
	QPointF pinPos;
	int cell_counter;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;

	blif::BLIFData *blifdata;
	BLIFDataComponentInfo comp;

	celpath = QDir(getLayoutDir()).filePath(top+".cel");
	QFile celFile(celpath);
	celFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if(!celFile.isOpen()){
		qDebug() << "- Error, unable to open " << celpath << " for output";
		return;
	}
	QTextStream outStream(&celFile);

	blifpath = QDir(getSynthesisDir()).filePath(top+".blif");
	blifdata = new blif::BLIFData(blifpath);
	cell_counter = 0;
	foreach(comp, blifdata->getComponents()) {
		compname = comp.getName();
		if(!macroCounters.contains(compname))
			macroCounters[compname] = 0;

		if(macro=getMacro(compname)) {
			celfileline = "cell ";
			celfileline += QString::number(cell_counter);
			celfileline += " ";
			celfileline += compname;
			celfileline += "_";
			celfileline += QString::number(macroCounters[compname]);
			outStream << celfileline;
			outStream << endl;

			mult = getBaseUnits(compname);
			w = macro->getWidth();
			w *= mult;
			h = macro->getHeight();
			h *= mult;
			celfileline = "left ";
			celfileline += QString::number(-w/2);
			celfileline += " right ";
			celfileline += QString::number(w/2);
			celfileline += " bottom ";
			celfileline += QString::number(-h/2);
			celfileline += " top ";
			celfileline += QString::number(h/2);
			outStream << celfileline;
			outStream << endl;

			foreach(pinname, comp.getPins()) {
				pin = macro->getPin(pinname);
				pinPos = pin?pin->getCenter():QPointF(0,0);

				celfileline = "pin name ";
				celfileline += pinname;
				celfileline += " signal ";
				celfileline += comp.getSignal(pinname);
				celfileline += " layer 1 ";
				celfileline += QString::number(mult*pinPos.x()-w/2);
				celfileline += " ";
				celfileline += QString::number(mult*pinPos.y()-h/2);
				outStream << celfileline;
				outStream << endl;
			}

			cell_counter++;
			macroCounters[compname]++;
		}
	}

	int pad_counter = 0;
	QString padPin;
	foreach(padPin, blifdata->getPadPins()) {
		pad_counter++;
		outStream << endl;
		outStream << "pad " << QString::number(pad_counter) << " name twpin_" << padPin;
		outStream << endl;
		outStream << "corners 4 -800 -1000 -800 1000 800 1000 800 -1000";
		outStream << endl;
		outStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
		outStream << endl;
	}

	celFile.close();
}

void Project::place2def(QString top)
{
	QString defpath;
	QString pl1path;
	QString pl2path;
	QString pinpath;
	QString infopath;
	QString mcelpath;

	qreal dieHeight = getSmallestUnit()*100;
	qreal dieWidth = getSmallestUnit()*100;
	qreal dieOrigX = 0;
	qreal dieOrigY = 0;

	defpath = QDir(getLayoutDir()).filePath(top+".def");
	pl1path = QDir(getLayoutDir()).filePath(top+".pl1");
	pl2path = QDir(getLayoutDir()).filePath(top+".pl2");
	pinpath = QDir(getLayoutDir()).filePath(top+".pin");
	infopath = QDir(getLayoutDir()).filePath(top+".info");
	mcelpath = QDir(getLayoutDir()).filePath(top+".mcel");

	QFile defFile(defpath);
	defFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if(!defFile.isOpen()){
		qDebug() << "- Error, unable to open " << defpath << " for output";
		return;
	}

	QFile pl1File(pl1path);
	pl1File.open(QIODevice::ReadOnly | QIODevice::Text);
	if(!pl1File.isOpen()){
		qDebug() << "- Error, unable to open " << pl1path << " for output";
		defFile.close();
		return;
	}

	QFile pl2File(pl2path);
	pl2File.open(QIODevice::ReadOnly | QIODevice::Text);
	if(!pl2File.isOpen()){
		qDebug() << "- Error, unable to open " << pl2path << " for output";
		defFile.close();
		pl1File.close();
		return;
	}

	QFile pinFile(pinpath);
	pinFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(!pl1File.isOpen()){
		qDebug() << "- Error, unable to open " << pinpath << " for output";
		defFile.close();
		pl1File.close();
		pl2File.close();
		return;
	}

	QFile infoFile(infopath);
	infoFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(!infoFile.isOpen()){
		qDebug() << "- Error, unable to open " << infopath << " for output";
		defFile.close();
		pl1File.close();
		pl2File.close();
		pinFile.close();
		return;
	}

	QFile mcelFile(mcelpath);
	mcelFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(mcelFile.isOpen()){
		QTextStream mcelStream(&mcelFile);
		QStringList mcelLine;
		while (!mcelStream.atEnd()) {
			mcelLine = mcelStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(mcelLine[0]=="corners") {
				dieHeight=mcelLine[5].toDouble();
				dieWidth=mcelLine[6].toDouble();
				dieOrigX=mcelLine[2].toDouble();
				dieOrigY=mcelLine[3].toDouble();
				break;
			}
		}
		mcelFile.close();
	}

	QTextStream pl1Stream(&pl1File);
	QTextStream pl2Stream(&pl2File);
	QTextStream pinStream(&pinFile);
	QTextStream infoStream(&infoFile);
	QTextStream outStream(&defFile);

	outStream << "VERSION 5.6 ;";
	outStream << endl;

	outStream << "NAMESCASESENSITIVE ON ;";
	outStream << endl;

	outStream << "DIVIDERCHAR ";
	outStream << getDivideChar();
	outStream << " ; ";
	outStream << endl;

	outStream << "BUSBITCHARS ";
	outStream << getSubBitChar();
	outStream << " ; ";
	outStream << endl;

	outStream << "DESIGN ";
	outStream << top;
	outStream << " ;";
	outStream << endl;

	outStream << "UNITS DISTANCE MICRONS ";
	outStream << QString::number(getSmallestUnit());
	outStream << " ;";
	outStream << endl;
	outStream << endl;
	outStream << "DIEAREA ( ";
	outStream << QString::number(dieOrigX);
	outStream << " ";
	outStream << QString::number(dieOrigY);
	outStream << " ) ( ";
	outStream << QString::number(dieWidth);
	outStream << " ";
	outStream << QString::number(dieHeight);
	outStream << " ) ;";
	outStream << endl;
	outStream << endl;

	QStringList infoLine;
	QString layerName;
	qreal layerPitch;
	int layer_count = 0;
	while (!infoStream.atEnd()) {
		infoLine = infoStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(infoLine.count()>4) {
			layerName = infoLine[0];
			layerPitch = infoLine[1].toDouble();
			layerPitch *= getSmallestUnit();

			outStream << "TRACKS ";
			outStream << ((layer_count%2)?"X":"Y");
			outStream << " 0.0 DO ";
			outStream << QString::number(layerPitch/2);
			outStream << " STEP ";
			outStream << QString::number(layerPitch/4);
			outStream << " LAYER ";
			outStream << layerName;
			outStream << " ;";
			outStream << endl;

			layer_count++;
		}
	}
	infoFile.close();

	outStream << endl;
	outStream << endl;

	int count_components = 0;
	QStringList pl1list;
	QString outString;
	QString pl1line;
	QRegExp rx("(.+)\_[0-9]+");
	QString instance_name;
	QString instance_type;
	qreal px, py;

	while (!pl1Stream.atEnd()) {
		pl1line = pl1Stream.readLine();
		pl1list = pl1line.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(pl1list.count()==7) {
			instance_name = pl1list.at(0);
			instance_type = instance_name;
			rx.indexIn(instance_name);
			instance_type = rx.cap(1);

			px = pl1list.at(1).toInt();
			py = pl1list.at(2).toInt();

			outString += "- ";
			outString += instance_name;
			outString += " ";
			outString += instance_type;
			outString += " + PLACED ( ";
			outString += QString::number(px);
			outString += " ";
			outString += QString::number(py);
			outString += " ) ";

			switch(pl1list.at(5).toInt()) {
				case 0:
					outString += "N";
					break;
				case 1:
					outString += "FS";
					break;
				case 2:
					outString += "FN";
					break;
				case 3:
					outString += "S";
					break;
				case 4:
					outString += "FE";
					break;
				case 5:
					outString += "FW";
					break;
				case 6:
					outString += "W";
					break;
				case 7:
					outString += "E";
					break;
			}

			outString += " ;\n";
			count_components++;
		}
	}

	outStream << "COMPONENTS ";
	outStream << QString::number(count_components);
	outStream << " ; ";
	outStream << endl;
	outStream << outString;
	outStream << "END COMPONENTS";
	outStream << endl;
	outStream << endl;

	QStringList pinLine;
	QString pinName;
	QString pinString = "\n";
	QString specialNetString = "\n";
	QRegExp twfilter("twpin\_(.+)+");
	int pin_count = 0;
	int pinx, piny;
	while (!pl2Stream.atEnd()) {
		pinLine = pl2Stream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(pinLine.count()>1) {
			twfilter.indexIn(pinLine.at(0));
			pinName = twfilter.cap(1);
			if(pinName!=QString()) {
				pinx = pinLine.at(1).toInt();
				piny = pinLine.at(2).toInt();
				pinString += "- " + pinName + " + NET " + pinName;
				pinString += "\n";
				pinString += "  + LAYER metal2 ( 0 0 ) ( 1 1 )";
				pinString += "\n";
				pinString += "  + PLACED ( ";
				pinString += QString::number(pinx);
				pinString += " ";
				pinString += QString::number(piny);
				pinString += " ) N ;";
				pinString += "\n";

				specialNetString += "- " + pinName + " ;\n";

				pin_count++;
			}
		}
	}
	pl2File.close();

	outStream << "PINS ";
	outStream << QString::number(pin_count);
	outStream << " ;";
	outStream << endl;
	outStream << pinString;
	outStream << "END PINS";
	outStream << endl;
	outStream << endl;

	outStream << "SPECIALNETS ";
	outStream << QString::number(pin_count);
	outStream << " ;";
	outStream << endl;
	outStream << specialNetString;
	outStream << endl;
	outStream << "END SPECIALNETS";
	outStream << endl;
	outStream << endl;

	int nets_counter;
	QMap<QString,QStringList> mapping;

	nets_counter = 0;
	while (!pinStream.atEnd()) {
		pinLine = pinStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(pinLine.count()>3) {
			if(pinLine.at(2)=="twfeed")
				continue;
			mapping[pinLine.at(0)].append(pinLine.at(2)+" "+pinLine.at(3));
			nets_counter++;
		}
	}
	pinFile.close();

	outString = "";
	foreach(QString sig, mapping.keys()) {
		outString += "- ";
		outString += sig;
		outString += '\n';
		foreach(QString port, mapping[sig]) {
			outString += "  ( ";
			outString += port;
			outString += " )";
			outString += '\n';
		}
		outString += ";\n";
	}

	outStream << "NETS ";
	outStream << QString::number(nets_counter);
	outStream << " ;";
	outStream << endl;
	outStream << outString;
	outStream << "END NETS";
	outStream << endl;
	outStream << endl;

	outStream << "END DESIGN";
	outStream << endl;

	pl1File.close();
	defFile.close();
}

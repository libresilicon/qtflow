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
	QTextStream defStream(&celFile);

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
			defStream << celfileline;
			defStream << endl;

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
			defStream << celfileline;
			defStream << endl;

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
				defStream << celfileline;
				defStream << endl;
			}

			cell_counter++;
			macroCounters[compname]++;
		}
	}

	// side restrictions: T,B,L,R
	int pad_counter = 0;
	QString padPin;
	foreach(padPin, blifdata->getPadPinsInput()) {
		pad_counter++;
		defStream << endl;
		defStream << "pad " << QString::number(pad_counter) << " name twpin_" << padPin;
		defStream << endl;
		defStream << "corners 4 -1000 -1000 -1000 1000 1000 1000 1000 -1000";
		defStream << " restrict side L";
		defStream << endl;
		defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
		defStream << endl;
	}
	foreach(padPin, blifdata->getPadPinsOutput()) {
		pad_counter++;
		defStream << endl;
		defStream << "pad " << QString::number(pad_counter) << " name twpin_" << padPin;
		defStream << endl;
		defStream << "corners 4 -1000 -1000 -1000 1000 1000 1000 1000 -1000";
		defStream << " restrict side R";
		defStream << endl;
		defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
		defStream << endl;
	}

	celFile.close();
}

void Project::place2def(QString top)
{
	QString defpath;
	QString pl1path;
	QString pl2path;
	QString pinpath;
	QString mcelpath;
	QString mverpath;
	QString outpath;

	QString layerName;
	qreal layerPitch;

	qreal dieHeight = getSmallestUnit()*100;
	qreal dieWidth = getSmallestUnit()*100;
	qreal gridHeight = getSmallestUnit()*100;
	qreal gridWidth = getSmallestUnit()*100;
	qreal dieOrigX = 0.1;
	qreal dieOrigY = 0.1;
	qreal pitch = 200;
	QMap<QString,QStringList> mapping;

	QStringList lineString;
	QString instance_name;
	QString instance_type;

	defpath = QDir(getLayoutDir()).filePath(top+".def");
	pl1path = QDir(getLayoutDir()).filePath(top+".pl1");
	pl2path = QDir(getLayoutDir()).filePath(top+".pl2");
	pinpath = QDir(getLayoutDir()).filePath(top+".pin");
	mcelpath = QDir(getLayoutDir()).filePath(top+".mcel");
	mverpath = QDir(getLayoutDir()).filePath(top+".mver");
	outpath = QDir(getLayoutDir()).filePath(top+".out");

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

	QFile mverFile(mverpath);
	mverFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(mverFile.isOpen()){
		QTextStream mverStream(&mverFile);
		while (!mverStream.atEnd()) {
			lineString = mverStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(lineString[0]=="core") {
				dieOrigX=lineString[1].toDouble();
				dieOrigY=lineString[2].toDouble();
				dieHeight=lineString[3].toDouble();
				dieWidth=lineString[4].toDouble();
			} else if(lineString[0]=="grid") {
				gridWidth = lineString[1].toDouble();
				gridHeight = lineString[2].toDouble();
			}
		}
		mverFile.close();
	}

	QFile outFile(outpath);
	outFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(outFile.isOpen()){
		QTextStream outStream(&mverFile);
		while (!outStream.atEnd()) {
			lineString = outStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(lineString[0].contains("track.pitch")) {
				pitch = lineString[1].toDouble();
				pitch *= getSmallestUnit();
				break;
			}
		}
		outFile.close();
	}

	QTextStream pl1Stream(&pl1File);
	QTextStream pl2Stream(&pl2File);
	QTextStream pinStream(&pinFile);
	QTextStream defStream(&defFile);

	QString netName;
	QString pinName;
	QString componentName;
	qreal pinx, piny, pinw, pinh;

	QString pinString;
	QString netString;
	QString tracksString;
	QString componentString;

	QMap<QString,QString> pinLayerMapping;
	QMap<QString,QString> pinNetMapping;

	int pin_count = 0;
	int layer_count = 0;
	int count_components = 0;

	QRegExp pl1RX("(.+)\_[0-9]+");
	QRegExp pl1RXB("twpin\_(.+)+");

	while (!pinStream.atEnd()) {
		lineString = pinStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(lineString.count()>3) {

			netName = lineString.at(0);
			componentName = lineString.at(2);
			pinName = lineString.at(3);
			pinx = lineString.at(4).toInt();
			piny = lineString.at(5).toInt();

			pinNetMapping[pinName] = netName;

			pinLayerMapping[pinName] = getRoutingLayers().at(lineString.at(8).toInt());

			pl1RXB.indexIn(componentName);
			if(pl1RXB.cap(1)==QString()) {
				if((componentName!="PSEUDO_CELL")&&(componentName!="twfeed")) {
					mapping[netName].append(componentName+" "+pinName);
				}
			}
		}
	}

	foreach(QString sig, mapping.keys()) {
		netString += "\n- "+sig;
		foreach(QString port, mapping[sig]) {
			netString += "\n  ( " + port +  " )";
		}
		netString += " ;";
	}

	while (!pl1Stream.atEnd()) {
		lineString = pl1Stream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(lineString.count()==7) {
			instance_name = lineString.at(0);

			pl1RXB.indexIn(instance_name);
			instance_type = pl1RXB.cap(1);
			if(instance_type!=QString()) continue;

			pl1RX.indexIn(instance_name);
			instance_type = pl1RX.cap(1);

			pinx = lineString.at(1).toInt();
			piny = lineString.at(2).toInt();

			componentString +="- "+instance_name+" "+instance_type+" + PLACED ( "+QString::number(pinx)+" "+QString::number(piny)+" ) ";

			switch(lineString.at(5).toInt()) {
				case 0:
					componentString += "N";
					break;
				case 1:
					componentString += "FS";
					break;
				case 2:
					componentString += "FN";
					break;
				case 3:
					componentString += "S";
					break;
				case 4:
					componentString += "FE";
					break;
				case 5:
					componentString += "FW";
					break;
				case 6:
					componentString += "W";
					break;
				case 7:
					componentString += "E";
					break;
			}

			componentString += " ;\n";
			count_components++;
		}
	}

	QRegExp pl2RX("twpin\_(.+)+");
	while(!pl2Stream.atEnd()) {
		lineString = pl2Stream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
		if(lineString.count()>2) {

			pl2RX.indexIn(lineString.at(0));
			pinName = pl2RX.cap(1);

			netName = pinNetMapping[pinName];

			if(pinName!=QString()) {
				pinw = lineString.at(3).toDouble() - lineString.at(1).toDouble();
				pinh = lineString.at(4).toDouble() - lineString.at(2).toDouble();

				pinx = lineString.at(1).toDouble();
				pinx += pinw/2;

				piny = lineString.at(2).toDouble();
				piny += pinh/2;

				//if( pinx < 0 ) pinx = 0;
				//if( piny < 0 ) piny = 0;

				pinString += "- "+pinName+" + NET "+netName+"\n";
				pinString += "  + LAYER ";
				pinString += pinLayerMapping[pinName];
				pinString += " ( "+QString::number(-pinw/2)+" "+QString::number(-pinh/2)+" ) ( "+QString::number(pinw/2)+" "+QString::number(pinh/2)+" )\n";
				pinString += "  + PLACED ( ";
				pinString += QString::number(pinx);
				pinString += " ";
				pinString += QString::number(piny);
				pinString += " ) ";
				pinString += "N";
				pinString += " ;\n";

				pin_count++;
			}
		}
	}

	tracksString = "";
	foreach(layerName, getRoutingLayers()) {
		layerPitch = 1000;
		tracksString += "TRACKS "+QString((layer_count%2)?"X":"Y")+" ";
		tracksString += ((layer_count%2)?QString::number(dieOrigX):QString::number(dieOrigY));
		tracksString += " DO "+QString::number(gridWidth);
		tracksString += " STEP "+QString::number(gridHeight);
		tracksString += " LAYER "+layerName+" ;\n";
		layer_count++;
	}

	pinFile.close();
	pl1File.close();
	pl2File.close();

	defStream << "VERSION 5.6 ;" << endl;
	defStream << "NAMESCASESENSITIVE ON ;" << endl;
	defStream << "DIVIDERCHAR " << getDivideChar() << " ;" << endl;
	defStream << "BUSBITCHARS " << getSubBitChar() << " ;" << endl;
	defStream << "DESIGN " << top << " ;" << endl;
	defStream << "UNITS DISTANCE MICRONS " << QString::number(getSmallestUnit()) << " ;" << endl;
	defStream	<< "DIEAREA ( " << QString::number(dieOrigX) << " " << QString::number(dieOrigY) << " )"
				<< " ( " << QString::number(dieWidth) << " " << QString::number(dieHeight) << " ) ;" << endl;

	defStream << endl << tracksString << endl;

	defStream << "COMPONENTS " << QString::number(count_components) << " ; " << endl;
	defStream << componentString;
	defStream << "END COMPONENTS" << endl << endl;

	defStream << "PINS " << QString::number(pin_count) << " ;" << endl;
	defStream << pinString;
	defStream << "END PINS" << endl << endl;

	defStream << "NETS " << QString::number(mapping.count()) << " ;";
	defStream << netString << endl;
	defStream << "END NETS" << endl << endl;

	defStream << "END DESIGN" << endl;

	defFile.close();
}

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
	QString padPin;
	QStringList sides;
	sides << "T" << "B" << "L" << "R";
	int pad_counter = 0;

	foreach(padPin, blifdata->getPadPinsInput()) {
		pad_counter++;
		defStream << endl;
		defStream << "pad " << QString::number(pad_counter) << " name twpin_" << padPin;
		defStream << endl;
		defStream << "corners 4 -500 -500 -500 500 500 500 500 -500";
		defStream << " restrict side B";
		defStream << endl;
		defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
		defStream << endl;
	}
	foreach(padPin, blifdata->getPadPinsOutput()) {
		pad_counter++;
		defStream << endl;
		defStream << "pad " << QString::number(pad_counter) << " name twpin_" << padPin;
		defStream << endl;
		defStream << "corners 4 -500 -500 -500 500 500 500 500 -500";
		defStream << " restrict side T";
		defStream << endl;
		defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
		defStream << endl;
	}
	foreach(padPin, blifdata->getPadPinsPower()) {
		foreach(QString side, sides) {
			pad_counter++;
			defStream << endl;
			defStream << "pad " << QString::number(pad_counter) << " name twpin_" << side << "." << padPin;
			defStream << endl;
			defStream << "corners 4 -500 -500 -500 500 500 500 500 -500";
			defStream << " restrict side " << side;
			defStream << endl;
			defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
			defStream << endl;
		}
	}
	foreach(padPin, blifdata->getPadPinsGround()) {
		foreach(QString side, sides) {
			pad_counter++;
			defStream << endl;
			defStream << "pad " << QString::number(pad_counter) << " name twpin_" << side << "." << padPin;
			defStream << endl;
			defStream << "corners 4 -500 -500 -500 500 500 500 500 -500";
			defStream << " restrict side " << side;
			defStream << endl;
			defStream << "pin name " << padPin << " signal " << padPin << " layer 1 0 0";
			defStream << endl;
		}
	}

	celFile.close();
}

void Project::generatecfg(QString top)
{
	QString outpath = QDir(getLayoutDir()).filePath(top+".cfg");
	QFile outFile(outpath);
	outFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if(!outFile.isOpen()){
		qDebug() << "- Error, unable to open " << outpath << " for output";
		return;
	}
	QTextStream outStream(&outFile);
	outStream << "verbose 1" << endl;
	foreach(QString lefn, getLibraryFiles()) {
		outStream << "read_lef " << lefn << endl;
	}

	outStream << "read_def " << top << ".def" << endl;

	outStream << "vdd " << "vdd" << endl;
	outStream << "gnd " << "gnd" << endl;

	outStream << "read_def " << top << ".def" << endl;
	outStream << "qrouter::standard_route " << top << "_route.def true" << endl;
	outStream << "qrouter::write_delays " << top << ".rc" << endl;
	outStream << "quit" << endl;

	outFile.close();
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

	qreal gridy = getSmallestUnit()*100;
	qreal gridx = getSmallestUnit()*100;
	qreal die_x1, die_x2;
	qreal die_y1, die_y2;
	qreal pinx, piny, pinw, pinh;
	qreal pitch = 200;
	QMap<QString,QStringList> mapping;

	qreal xbot = getSmallestUnit();
	qreal ybot = getSmallestUnit();
	qreal cellxbot = getSmallestUnit();
	qreal cellybot = getSmallestUnit();

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
				die_x1=lineString[1].toDouble();
				die_y1=lineString[2].toDouble();
				die_x2=lineString[3].toDouble();
				die_y2=lineString[4].toDouble();
			} else if(lineString[0]=="grid") {
				gridx = lineString[1].toDouble();
				gridy = lineString[2].toDouble();
			}
		}
		mverFile.close();
	}

	int corner_count = 0;
	int dx, dy;
	QFile mcelFile(mcelpath);
	mcelFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if(mcelFile.isOpen()){
		QTextStream mcelStream(&mcelFile);
		while (!mcelStream.atEnd()) {
			lineString = mcelStream.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
			if(lineString.count()) {
				if(lineString[0]=="pad") // pad section reached
					break;
				if(lineString[0]=="corners") {
					corner_count = lineString[1].toInt();
					for(int i=0; i<corner_count; i++) {
						pinx = lineString[2+i*2].toInt();
						piny = lineString[3+i*2].toInt();
					}
				}
			}
		}
		mcelFile.close();
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

			if(instance_type!=QString())
				continue;

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

				layerName = pinLayerMapping[pinName];
				if(layerName==QString())
					layerName = getRoutingLayers().at(0);

				if(netName==QString())
					netName = pinName;

				pinx = lineString.at(1).toDouble();
				//pinx += pinw/2;

				piny = lineString.at(2).toDouble();
				//piny += pinh/2;

				pinString += "- "+pinName+" + NET "+netName+"\n";
				pinString += "  + LAYER ";
				pinString += layerName;
				pinString += " ( 0 0 ) ( "+QString::number(pinw)+" "+QString::number(pinh)+" )\n";
				//pinString += " ( 0 0 ) ( 1 1 )\n";
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
		tracksString += "TRACKS "+QString((layer_count%2)?"X":"Y")+" ";
		tracksString += ((layer_count%2)?QString::number(die_x1):QString::number(die_y1));
		tracksString += " DO "+QString::number(gridx);
		tracksString += " STEP "+QString::number(gridy);
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
	defStream	<< "DIEAREA ( " << QString::number(die_x1) << " " << QString::number(die_y1) << " )"
				<< " ( " << QString::number(die_x2) << " " << QString::number(die_y2) << " ) ;" << endl;

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

#include "project.h"

void Project::schematics2blif(QString top)
{
}

void Project::blif2cel(QString top)
{
	QString blifpath;
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

		if(macro=getMacro(compname)) {
			celfileline = "cell ";
			celfileline += QString::number(cell_counter);
			celfileline += " ";
			celfileline += compname;
			qDebug() << celfileline;
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
			qDebug() << celfileline;
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

				qDebug() << celfileline;
				outStream << celfileline;
				outStream << endl;
			}

			cell_counter++;
		}
	}
	celFile.close();
}

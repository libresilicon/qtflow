#include "project.h"

void Project::buildPadFrame()
{
	lef::LEFMacro* macro;
	QString cellName;
	QMap<QString,qreal> cellNameCounter;
	QStringList sides;
	qreal longestSide;
	QMap<QString,qreal> sideLengths;
	QString padinfoPath = getPadInfoFile();
	QString padframePath = getPadFrameFile();
	sides << "T";
	sides << "B";
	sides << "L";
	sides << "R";
	if(QFileInfo(padinfoPath).exists()) {
		PadInfo padinfo(padinfoPath);

		// getting maximum dimensions
		foreach(QString s, sides) {
			sideLengths[s]=0;
			foreach(QString pad, padinfo.getPadSide(s)) {
				cellName = padinfo.getPadCell(pad);
				macro = getMacro(cellName);
				if(macro) {
					if((s=="T")||(s=="B")) sideLengths[s]+=macro->getWidth();
					if((s=="L")||(s=="R")) sideLengths[s]+=macro->getHeight();
				}
				cellNameCounter[cellName]=1;
			}
		}

		longestSide = 0;
		foreach(QString k, sideLengths.keys()) {
			if(longestSide<sideLengths[k]) longestSide = sideLengths[k];
		}

		if(QFileInfo(padframePath).exists()) QFile(padframePath).rename(padframePath+".bak"); // back up
		QFile outFile(padframePath);
		if (outFile.open(QIODevice::WriteOnly)) {
			QTextStream out(&outFile);
			out << "magic" << endl;
			out << "tech " << getTechnology() << endl;
			out << "magscale 1 2" << endl;
			out << "timestamp " << QDateTime::currentMSecsSinceEpoch() << endl << endl;

			qreal x,y;
			qreal lastw, lasth;
			foreach(QString s, sides) {
				x = 1;
				y = 1;
				lastw = 0;
				lasth = 0;
				foreach(QString pad, padinfo.getPadSide(s)) {
					cellName = padinfo.getPadCell(pad);
					macro = getMacro(cellName);
					if(macro) {
						out << "use " << cellName << "  " << cellName << "_" << cellNameCounter[cellName] << endl;
						out << "timestamp " << QDateTime::currentMSecsSinceEpoch() << endl;
						if((s=="T")||(s=="B")) {
							x+=lastw;
							y=(s=="B")?1:longestSide;
						}
						if((s=="L")||(s=="R")) {
							x=(s=="L")?1:longestSide;
							y+=lasth;
						}
						out << "transform 1 0 " << x << " 0 1 " << y << endl;
						out << "box 0 0 " << macro->getWidth() << " " << macro->getHeight() << endl << endl;
						cellNameCounter[cellName]++;
						lastw = macro->getWidth();
						lasth = macro->getHeight();
					}
				}
			}

			outFile.close();
		}

		/*if(QFile(getPadFrameScript()).exists()) {
			mainContext.evalFile(getPadFrameScript());
		} else {
			mainContext.evalScript("print \"not defined\"");
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

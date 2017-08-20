#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <iostream>

int main(int argc, char *argv[])
{
	bool noGraphics;
	QApplication app(argc, argv);
	MainWindow w;
	QCommandLineParser parser;
	parser.setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser.addHelpOption();
	parser.addVersionOption();
	QCommandLineOption doGraphicsO(QStringList() << "n" << "no-graphics", QCoreApplication::translate("main", "Run without graphics"));
	parser.addOption(doGraphicsO);
	parser.process(app);
	noGraphics = parser.isSet("no-graphics");
	if(noGraphics) {
		// do headless stuff
	} else {
		w.show();
		return app.exec();
	}
}

#include "mainwindow.h"
#include "headlessmainapp.h"

#include <QApplication>
#include <QtWidgets>
#include <QMainWindow>
#include <QCommandLineParser>

#include <iostream>

int main(int argc, char *argv[])
{
	bool noGraphics;
	QApplication app(argc, argv);
	QCommandLineParser parser;
	parser.setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser.addHelpOption();
	parser.addVersionOption();
	QCommandLineOption doGraphicsO(QStringList() << "n" << "no-graphics", QCoreApplication::translate("main", "Run without graphics"));
	parser.addOption(doGraphicsO);
	parser.process(app);
	noGraphics = parser.isSet("no-graphics");

	if(noGraphics) {
		HeadlessMainApp a(QDir(".").absolutePath());
		a.runSynthesis();
	} else {
		MainWindow *w = new MainWindow();
		w->show();
	}
	return app.exec();
}

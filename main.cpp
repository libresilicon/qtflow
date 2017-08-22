#include "mainwindow.h"
#include "headlessmainapp.h"

#include <QApplication>
#include <QtWidgets>
#include <QMainWindow>
#include <QCommandLineParser>

#include <iostream>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	HeadlessMainApp *ha;
	MainWindow *w;

	QCommandLineParser *parser = new QCommandLineParser();
	parser->setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser->addHelpOption();
	parser->addVersionOption();
	parser->addOption(QCommandLineOption(QStringList() << "n" << "no-graphics", QCoreApplication::translate("main", "Run without graphics")));
	parser->addOption(QCommandLineOption(QStringList() << "m" << "top-level", QCoreApplication::translate("main", "Top level module"),"1"));
	parser->addOption(QCommandLineOption(QStringList() << "s" << "run-synthesis", QCoreApplication::translate("main", "Run synthesis of module")));
	parser->addOption(QCommandLineOption(QStringList() << "p" << "run-placement", QCoreApplication::translate("main", "Run placement of module")));
	parser->addOption(QCommandLineOption(QStringList() << "r" << "run-routing", QCoreApplication::translate("main", "Run routing of module")));
	parser->addOption(QCommandLineOption(QStringList() << "t" << "technology", QCoreApplication::translate("main", "Use specific technology"),"1"));
	parser->process(app);

	if(parser->isSet("no-graphics")) {
		ha = new HeadlessMainApp(parser);
		ha->run();
	} else {
		w = new MainWindow();
		w->show();
		return app.exec();
	}
}

#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"

class MainApplication : public QApplication
{
	Q_OBJECT

public:
	MainApplication(int &argc, char **argv);
	int exec();

private:
	int runHeadless();
	bool isValidTechnology(QString technology);
	bool isValidProcess(QString process);
	int handleExit();

	MainWindow *w;
	QSettings *settings;
	Project *project;
	PythonQtObjectPtr mainContext;
	QCommandLineParser *parser;
	QDomDocument *settingsFileProcess;
	PythonQtScriptingConsole *console;
};

#endif // MAINAPPLICATION_H

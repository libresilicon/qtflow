#ifndef HEADLESSMAINAPP_H
#define HEADLESSMAINAPP_H

#include "project.h"
#include "common.h"
#include "edit.h"
#include "dependencies.h"
#include "modules.h"
#include "new.h"
#include "iopads.h"
#include "wave.h"
#include "welcome.h"

#include <iostream>
#include <string>

#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QString>
#include <QProcess>
#include <QSettings>
#include <QCommandLineParser>

class HeadlessMainApp : public QObject
{
	Q_OBJECT

public:
	HeadlessMainApp(QCommandLineParser * parser = nullptr);
	void run();
	void runSynthesis();
	void runPlacing();
	void runRouting();
	void setTechnology(QString tech);

private:
	Project *project;
	Dependencies *dependencies;
	QProcess *tcsh;
	New *createWidget;
	Welcome *welcomeWidget;
	Edit *editWidget;
	Wave *timingWidget;
	IOPads *iopads;
	Modules *modules;
	QSettings *settings;
	QCommandLineParser * parser;

signals:
	void finished();
};

#endif // HEADLESSMAINAPP_H

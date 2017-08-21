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

class HeadlessMainApp
{
public:
	HeadlessMainApp(QString path);
	void runSynthesis();
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
};

#endif // HEADLESSMAINAPP_H

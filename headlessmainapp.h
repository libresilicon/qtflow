#ifndef HEADLESSMAINAPP_H
#define HEADLESSMAINAPP_H

#include "app.h"
#include "project.h"
#include "session.h"
#include "common.h"
#include "edit.h"
#include "environment.h"
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

class HeadlessMainApp
{
public:
	HeadlessMainApp();
	void runSynthesis();
private:
	IProject *project;
	IDependencies *dependencies;
	QProcess *tcsh;
	New *createWidget;
	Welcome *welcomeWidget;
	Edit *editWidget;
	Wave *timingWidget;
	Environment *buildEnvironment;
	Environment *qtflowEnvironment;
	IOPads *iopads;
	Modules *modules;
	Session &session;
};

#endif // HEADLESSMAINAPP_H

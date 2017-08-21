#include "headlessmainapp.h"
#include "project.h"

#include <iostream>

HeadlessMainApp::HeadlessMainApp(QString path) :
	project(new Project),
	tcsh(new QProcess),
	createWidget(new New),
	welcomeWidget(new Welcome),
	editWidget(new Edit),
	timingWidget(new Wave),
	iopads(new IOPads),
	modules(new Modules)
{
	std::cout << "Work path is: " << path.toStdString() << std::endl;
	settings =  new QSettings(QSettings::IniFormat, QSettings::UserScope, ".qtflow", ".qtflow");
	dependencies = new Dependencies(settings);
}

void HeadlessMainApp::runSynthesis()
{
	//project->synthesis();
}

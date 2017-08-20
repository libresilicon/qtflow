#include "headlessmainapp.h"
#include "project.h"

HeadlessMainApp::HeadlessMainApp() :
	project(new Qflow),
	dependencies(new Dependencies),
	tcsh(new QProcess),
	createWidget(new New),
	welcomeWidget(new Welcome),
	editWidget(new Edit),
	timingWidget(new Wave),
	buildEnvironment(new Environment),
	qtflowEnvironment(new Environment),
	iopads(new IOPads),
	modules(new Modules),
	session(Session::Instance())
{

}

void HeadlessMainApp::runSynthesis()
{

}

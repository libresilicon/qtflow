#include "headlessmainapp.h"
#include "project.h"

HeadlessMainApp::HeadlessMainApp(QCommandLineParser * p ) :
	project(new Project),
	tcsh(new QProcess),
	createWidget(new New),
	welcomeWidget(new Welcome),
	editWidget(new Edit),
	timingWidget(new Wave),
	iopads(new IOPads),
	modules(new Modules)
{
	QString top;
	QString path = QDir(".").absolutePath();
	QTextStream(stdout) << QString("Work path is: ") << path << "\n";
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, ".qtflow", ".qtflow");
	parser = p;
	if(p) {
		if(parser->isSet("top-level")) {
			top = parser->value("top-level");
			project = new Project(path+'/'+top+".pro");
			project->setTopLevel(parser->value("top-level"));
		} else {
			qErrnoWarning("No top level given!");
			exit(0);
		}
		if(parser->isSet("technology")) project->setTechnology(parser->value("technology"));
	}
}

void HeadlessMainApp::runSynthesis()
{
	//project->synthesis();
}

void HeadlessMainApp::runPlacing()
{
	//project->synthesis();
}

void HeadlessMainApp::runRouting()
{
	//project->synthesis();
}

void HeadlessMainApp::run()
{
	if(parser) if (parser->isSet("run-synthesis")||parser->isSet("run-placement")||parser->isSet("run-routing")){
		if(parser->isSet("run-synthesis")) runSynthesis();
		if(parser->isSet("run-placement")) runPlacing();
		if(parser->isSet("run-routing")) runRouting();
	}
	emit finished();
	exit(0);
}

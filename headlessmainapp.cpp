#include "headlessmainapp.h"
#include "project.h"

HeadlessMainApp::HeadlessMainApp(QCommandLineParser * p, PythonQtObjectPtr *context) :
	mainContext(context)
{
	QString path = QDir(".").absolutePath();
	parser = p;

	QTextStream(stdout) << QString("Work path is: ") << path << "\n";
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	if(parser) {
		if(parser->isSet("top-level")) {
			project = new Project(settings, path+'/'+parser->value("top-level")+".pro", mainContext);
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
	QTextStream(stdout) << "Running synthesis...\n";
	project->synthesis();
}

void HeadlessMainApp::runPlacing()
{
	QTextStream(stdout) << "Running placement...\n";
	//project->synthesis();
}

void HeadlessMainApp::runRouting()
{
	QTextStream(stdout) << "Running routing...\n";
	//project->synthesis();
}

void HeadlessMainApp::run()
{
	if(parser) {
		if (parser->isSet("run-synthesis")||parser->isSet("run-placement")||parser->isSet("run-routing")){
			if(parser->isSet("run-synthesis")) runSynthesis();
			if(parser->isSet("run-placement")) runPlacing();
			if(parser->isSet("run-routing")) runRouting();
		} else {
			runSynthesis();
			runPlacing();
			runRouting();
		}
	} else {
		runSynthesis();
		runPlacing();
		runRouting();
	}
	emit finished();
}

#include "headlessmainapp.h"
#include "project.h"

HeadlessMainApp::HeadlessMainApp(QCommandLineParser * p ) :
	tcsh(new QProcess),
	createWidget(new New),
	welcomeWidget(new Welcome),
	editWidget(new Edit),
	timingWidget(new Wave),
	iopads(new IOPads),
	modules(new Modules)
{
	QString top;
	QString technology;
	QString project_name;
	QString path = QDir(".").absolutePath();
	parser = p;

	QTextStream(stdout) << QString("Work path is: ") << path << "\n";
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, ".qtflow", ".qtflow");
	if(parser) {
		if(parser->isSet("top-level")) {
			top = parser->value("top-level");
			technology = parser->isSet("technology")?parser->value("technology"):"osu035";
			project_name = path+'/'+top+".pro";
			if(!QFile(project_name).exists()) {
				QTextStream(stdout) << QString("Creating project: ") << project_name << "\n";
				QSettings project_settings(project_name, QSettings::NativeFormat);
				project_settings.setValue("technology", technology);
				project_settings.setValue("toplevel", top);
				project_settings.sync();
			}
			project = new Project(project_name);
			project->setRootDir(path);
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

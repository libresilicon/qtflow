#include "project.h"
#include "pyprojectsettings.h"

IProject::IProject() : QObject()
{
}

Project::Project(QSettings *s, QString path, PythonQtObjectPtr *main) :
	IProject(),
	settings(s),
	mainContext(main)
{
	executable
		= QFileDevice::ReadOwner
		| QFileDevice::WriteOwner
		| QFileDevice::ExeOwner
		| QFileDevice::ReadGroup
		| QFileDevice::WriteGroup
		| QFileDevice::ExeGroup
		| QFileDevice::ReadOther;

	if(QFile(path).exists()) {
		project_settings = new QSettings(path, QSettings::NativeFormat);
		if(project_settings->value("sourcedir","").toString()=="") {
			QTextStream(stdout) << "No variable called sourcedir set!!\n";
		}
		if(project_settings->value("projectType","").toString()=="") {
			QTextStream(stdout) << "No variable called projectType set!!\n\tSetting to default";
			project_settings->setValue("projectType","asic_mixed");
			project_settings->sync();
		}
	} else {
		create(path);
	}
	mainContext->addObject("project_settings", new PyProjectSettings(this));

	settings->beginGroup("history");
	QStringList recentProjectsList = settings->value("recentProjects").toStringList();
	recentProjectsList.append(path);
	recentProjectsList.removeDuplicates();
	settings->setValue("recentProjects",recentProjectsList);
	settings->endGroup();
	settings->sync();
}

Project::~Project()
{
	delete project_settings;
}

QString Project::getSourceDir()
{
	return project_settings->value("sourcedir").toString();
}

QString Project::getSynthesisDir()
{
	return project_settings->value("synthesis").toString();
}

QString Project::getRootDir()
{
	return project_settings->value("rootdir").toString();
}

QString Project::getTopLevel()
{
	return project_settings->value("toplevel").toString();
}

QString Project::getTestBench()
{
	return project_settings->value("testbench").toString();
}

QString Project::getLayoutDir()
{
	return project_settings->value("layout").toString();
}

QString Project::getVCDFile()
{
	return this->getTestBench()+".vcd";
}

QString Project::getVCDPath()
{
	return this->getSynthesisDir()+'/'+this->getVCDFile();
}

QString Project::getTechnology()
{
	return project_settings->value("technology").toString();
}

QString Project::getProcess()
{
	return project_settings->value("process").toString();
}

QString Project::getProjectType()
{
	return project_settings->value("projectType").toString();
}

void Project::setTestBench(QString top)
{
	project_settings->setValue("testbench",top);
	project_settings->sync();
}

void Project::setTopLevel(QString top)
{
	project_settings->setValue("toplevel",top);
	project_settings->sync();
}

void Project::setTechnology(QString tech)
{
	project_settings->setValue("technology",tech);
	project_settings->sync();
}

void Project::setProcess(QString proc)
{
	project_settings->setValue("process",proc);
	project_settings->sync();
}

void Project::setProjectType(QString proc)
{
	project_settings->setValue("projectType",proc);
	project_settings->sync();
}

void Project::create(QString path)
{
	QString rootdir;

	project_settings = new QSettings(path, QSettings::NativeFormat);
	project_settings->setValue("technology", "osu035");
	project_settings->sync();
	rootdir = QFileInfo(project_settings->fileName()).absolutePath();
	project_settings->setValue("rootdir", rootdir);
	project_settings->setValue("sourcedir", rootdir+"/source");
	project_settings->setValue("synthesis", rootdir+"/synthesis");
	project_settings->setValue("layout", rootdir+"/layout");
	project_settings->sync();

	QDir dir(rootdir);
	if(!QDir(rootdir+"/source").exists()) dir.mkdir("source");
	if(!QDir(rootdir+"/synthesis").exists()) dir.mkdir("synthesis");
	if(!QDir(rootdir+"/layout").exists()) dir.mkdir("layout");
}

void Project::synthesis()
{
	mainContext->evalFile(":/scripts/synthesis.py");
}

void Project::simulation()
{
	mainContext->evalFile(":/scripts/simulation.py");
}

void Project::placement()
{
}

void Project::routing()
{
}

void Project::buildAll()
{
}

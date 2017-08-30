#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QCommandLineParser>

#include <python/Python.h>

Project::Project(QSettings *s, QString path) :
	IProject(),
	settings(s)
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
	} else {
		create(path);
	}

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


QString Project::getTechnology()
{
	return project_settings->value("technology").toString();
}

QString Project::getProcess()
{
	return project_settings->value("process").toString();
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

bool Project::create(QString path)
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

	return true;
}

bool Project::synthesis()
{
	QTextStream(stdout) << QString("Running synthesis in ") << rootdir << "\n";
	QString fileName = ":/scripts/synthesis.js";
	QScriptEngine engine;
	QFile scriptFile(fileName);
	if (scriptFile.open(QIODevice::ReadOnly)) {
		QTextStream stream(&scriptFile);
		QString contents = stream.readAll();
		scriptFile.close();
		QScriptValue result = engine.evaluate(contents, fileName);
		QTextStream(stdout) << result.toString() << '\n';
	}
}

bool Project::simulation()
{
	QTextStream(stdout) << QString("Running simulation in ") << rootdir << "\n";
	QString fileName = ":/scripts/simulation.js";
	QScriptEngine engine;
	QFile scriptFile(fileName);
	if (scriptFile.open(QIODevice::ReadOnly)) {
		QTextStream stream(&scriptFile);
		QString contents = stream.readAll();
		scriptFile.close();
		QScriptValue result = engine.evaluate(contents, fileName);
		QTextStream(stdout) << result.toString() << '\n';
	}
}

bool Project::placement()
{
	return true;
}

bool Project::routing()
{
	return true;
}

bool Project::buildAll()
{
	return true;
}

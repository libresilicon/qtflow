#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QSettings>
#include <QProcess>
#include <QFileDevice>
#include <QCommandLineParser>
#include <QtScript>
#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QCommandLineParser>

#include <PythonQt.h>

class IProject
{
public:
	IProject() {}
	virtual ~IProject() {}
	virtual bool create(QString) { return false; }
	virtual bool simulation() { return false; }
	virtual bool synthesis() { return false; }
	virtual bool placement() { return false; }
	virtual bool routing() { return false; }
	virtual bool buildAll() { return false; }
};

class Project : public IProject
{
public:
	Project(QSettings *settings, QString path, PythonQtObjectPtr *main);
	~Project();
	bool create(QString);
	bool synthesis();
	bool simulation();
	bool placement();
	bool routing();
	bool buildAll();
	void setTechnology(QString tech);
	void setProcess(QString proc);
	void setTopLevel(QString mod);
	void setTestBench(QString mod);
	void setProjectType(QString proc);

	QString getSourceDir();
	QString getTopLevel();
	QString getRootDir();
	QString getTestBench();
	QString getTechnology();
	QString getProcess();
	QString getProjectType();

private:
	QSettings *settings;
	QSettings *project_settings;
	QString rootdir;
	PythonQtObjectPtr *mainModule;
	QFileDevice::Permissions executable;
};

#endif // PROJECT_H

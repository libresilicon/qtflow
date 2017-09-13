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
#include <QDomDocument>

#include <PythonQt.h>

class IProject: public QObject
{
	Q_OBJECT

public:
	IProject();
	virtual ~IProject() {}
	virtual void create(QString) {}
	virtual void simulation() {}
	virtual void synthesis() {}
	virtual void placement() {}
	virtual void routing() {}
	virtual void buildAll() {}
};

class Project : public IProject
{
	Q_OBJECT

public:
	Project(QSettings *settings, QString path, PythonQtObjectPtr *main);
	~Project();
	void create(QString);
	void synthesis();
	void simulation();
	void placement();
	void routing();
	void buildAll();
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
	QString getProcessFile();
	QString getProjectType();
	QString getSynthesisDir();
	QString getLayoutDir();
	QString getVCDFile();
	QString getVCDPath();

private:
	QSettings *settings;
	QSettings *project_settings;
	QString rootdir;
	PythonQtObjectPtr *mainContext;
	QFileDevice::Permissions executable;
	QDomDocument *settingsFileProcess;
};

#endif // PROJECT_H

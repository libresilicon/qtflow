#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QFile>
#include <QToolBar>
#include <QFileDialog>
#include <QTextStream>

#include <QXmlSimpleReader>
#include <QDomDocument>

#include "ui_projectsettings.h"

#include "project.h"

class ProjectSettings : public QDialog
{
	Q_OBJECT
public:
	explicit ProjectSettings(QWidget *parent = nullptr);
	void setProject(Project*);
	void setSettings(QSettings*s);
  
signals:
	
public slots:
	void technologyActivated();
	void processActivated();
	void storeData();

	void addDirectory();
	void deleteDirectory();

private:
	void setSearchPathList();
	void setSearchPathButtons();
	void setupEverything();

	Ui::ProjectSettings *ui;
	QSettings *settings;

	Project *project;
	QDomDocument *settingsFileProcess;
};

#endif // PROJECTSETTINGS_H

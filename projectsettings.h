#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QFile>

#include <QXmlSimpleReader>
#include <QDomDocument>

#include "ui_projectsettings.h"

class ProjectSettings : public QDialog
{
	Q_OBJECT
public:
	explicit ProjectSettings(QWidget *parent = nullptr);
  
signals:
	
public slots:
	void technologyActivated(int);
	void processActivated(int);

private:
	Ui::ProjectSettings *ui;

	QDomDocument *settingsFileProcess;
};

#endif // PROJECTSETTINGS_H

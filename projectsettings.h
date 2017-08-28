#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "ui_projectsettings.h"

class ProjectSettings : public QDialog
{
	Q_OBJECT
public:
	explicit ProjectSettings(QWidget *parent = nullptr);
  
signals:
	
public slots:
private:
	Ui::ProjectSettings *ui;
};

#endif // PROJECTSETTINGS_H

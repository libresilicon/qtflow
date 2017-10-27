#ifndef DRCSETTINGS_H
#define DRCSETTINGS_H

#include <QDialog>

#include "ui_drcsettings.h"

#include "project.h"

class DRCSettings : public QDialog
{
public:
	DRCSettings(QWidget *parent = Q_NULLPTR);
	void setProject(Project *p);

private:
	void refreshRuleList();

	Ui::DRCSettings *ui;
	Project *project;
};

#endif // DRCSETTINGS_H

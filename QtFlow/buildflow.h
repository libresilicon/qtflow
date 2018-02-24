#ifndef BUILDFLOW_H
#define BUILDFLOW_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QFile>
#include <QToolBar>
#include <QFileDialog>
#include <QTextStream>

#include <QXmlSimpleReader>
#include <QDomDocument>

#include "ui_buildflow.h"

#include "project.h"

class BuildFlow : public QDialog
{
	Q_OBJECT

public:
	explicit BuildFlow(QWidget *parent = nullptr);
	void setProject(Project*p);

protected:
	void displayBuildSteps();

private slots:
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();

private:
	Ui::BuildFlow *ui;
	Project *project;
};

#endif // BUILDFLOW_H

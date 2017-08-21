#include "templates.h"
#include "ui_templates.h"

#include "project.h"

#include <QFileDialog>

Templates::Templates(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Templates),
	project(new Project)
{
	ui->setupUi(this);
}

Templates::~Templates()
{
	delete ui;
	delete project;
}

void Templates::on_buttonBox_accepted()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory..."), ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (path == QString())
		return;

	project->create(path);
	//session.setProject(path);
	//session.getApp()->enableProject();
}

void Templates::on_buttonBox_rejected()
{

}

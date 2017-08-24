#include "projectselector.h"

ProjectSelector::ProjectSelector(QWidget *parent) :
	ui(new Ui::Projects),
	projects(new ProjectsTreeModel),
	QDockWidget(parent)
{
	ui->setupUi(this);
	ui->treeView->setModel(projects);
}

void ProjectSelector::setSourceDir(QString path)
{
	sourcedir = path;
	refresh();
}

void ProjectSelector::refresh()
{
	projects->setRootPath(sourcedir);
	ui->treeView->setModel(projects);
}

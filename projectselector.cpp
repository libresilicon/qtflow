#include "projectselector.h"
#include <QDirIterator>

ProjectSelector::ProjectSelector(QWidget *parent) :
	ui(new Ui::Projects),
	projects(new ProjectsTreeModel),
	QDockWidget(parent),
	project(NULL)
{
	ui->setupUi(this);
	ui->treeView->setModel(projects);
	context = new QMenu(ui->treeView);

	connect(ui->treeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onOpen(const QModelIndex&)));
}

void ProjectSelector::refresh()
{
	if(project) {
		projects->setRootPath(project->getRootDir());
		ui->treeView->setModel(projects);
	}
}

void ProjectSelector::onOpen(const QModelIndex &i)
{
	if(project) {
		QString filename = i.data().toString();
		QDirIterator it(project->getRootDir(), QDirIterator::Subdirectories);
		while (it.hasNext()) {
			it.next();
			if (it.fileName() == filename) {
				emit(openFile(it.filePath()));
			}
		}
	}
}

void ProjectSelector::onContextMenu(const QPoint &point)
{
	QModelIndex index = ui->treeView->indexAt(point);
	if (index.isValid())
		context->exec(ui->treeView->mapToGlobal(point));
}

void ProjectSelector::setProject(Project* p)
{
	project = p;
	refresh();
}

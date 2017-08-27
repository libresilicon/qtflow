#include "fileselector.h"

#include <QTextStream>

FileSelector::FileSelector(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::Files),
	filesystem(new QtFlowFileList),
	project(NULL)
{
	ui->setupUi(this);
	context = new QMenu(ui->listView);
	context->addAction("Open");
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
	connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onOpen(const QModelIndex&)));
}

void FileSelector::setProject(Project *p)
{
	project = p;
	refresh();
}

void FileSelector::refresh()
{
	if (project->getSourceDir() == QString())
		return;

	filesystem->setProject(project);
	ui->listView->setModel(filesystem);
	ui->listView->setRootIndex(filesystem->index(project->getSourceDir()));
}

void FileSelector::onSetTestBenchFile(QString m)
{
	QTextStream(stdout) << "New test bench " << m << '\n';
}

void FileSelector::onSetTopLevelFile(QString m)
{
	QTextStream(stdout) << "New top level " << m << '\n';
}

void FileSelector::onContextMenu(const QPoint &point)
{
	QModelIndex index = ui->listView->indexAt(point);
	if (index.isValid())
		context->exec(ui->listView->mapToGlobal(point));
}

void FileSelector::onOpen(const QModelIndex &i)
{
	emit(openFile(project->getSourceDir()+'/'+i.data().toString()));
}

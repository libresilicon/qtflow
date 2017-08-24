#include "fileselector.h"

#include <QTextStream>

FileSelector::FileSelector(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::Files),
	filesystem(new QFileSystemModel)
{
	ui->setupUi(this);
	filesContext = new QMenu(ui->listView);
	filesContext->addAction("Open");
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onFilesContextMenu(const QPoint&)));
	connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onFilesOpen(const QModelIndex&)));
}

void FileSelector::setSourceDir(QString path)
{
	sourcedir = path;
	refresh();
}

void FileSelector::refresh()
{
	if (sourcedir == QString())
		return;
	filesystem->setRootPath(sourcedir);
	ui->listView->setModel(filesystem);
	ui->listView->setRootIndex(filesystem->index(sourcedir));
}

void FileSelector::onFilesContextMenu(const QPoint &point)
{
	QModelIndex index = ui->listView->indexAt(point);
	if (index.isValid())
		filesContext->exec(ui->listView->mapToGlobal(point));
}

void FileSelector::onFilesOpen(const QModelIndex &i)
{
	emit(openFile(i.data().toString()));
}

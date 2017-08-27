#include "fileselector.h"

#include <QTextStream>

FileSelector::FileSelector(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::Files),
	filesystem(new QFileSystemModel)
{
	ui->setupUi(this);
	context = new QMenu(ui->listView);
	context->addAction("Open");
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
	connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onOpen(const QModelIndex&)));
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
	emit(openFile(sourcedir+'/'+i.data().toString()));
}

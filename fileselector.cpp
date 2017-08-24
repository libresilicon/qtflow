#include "fileselector.h"

#include <QTextStream>

FileSelector::FileSelector(QWidget *parent):
	ui(new Ui::Files),
	QDockWidget(parent),
	filesystem(new QFileSystemModel)
{
	ui->setupUi(this);
	filesContext = new QMenu(ui->listView);
	filesContext->addAction("Test 1");
	this->connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenuRequested(const QPoint&)));
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

void FileSelector::customContextMenuRequested(const QPoint &point)
{
	QTextStream(stdout) << "Test\n";
	//QModelIndex index = ui->listView->indexAt(point);
	//if (index.isValid())
	//	filesContext->exec(ui->listView->mapToGlobal(point));
}

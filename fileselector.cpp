#include "fileselector.h"

FileSelector::FileSelector(QWidget *parent):
	ui(new Ui::Files),
	QDockWidget(parent),
	filesystem(new QFileSystemModel)
{
	ui->setupUi(this);
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

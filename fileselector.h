#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QProcess>
#include <QSettings>

#include "ui_files.h"

class FileSelector : public QDockWidget
{
public:
	FileSelector(QWidget *parent);

	void refresh();

	void setSourceDir(QString path);

private:
	Ui::Files *ui;

	QFileSystemModel *filesystem;
	QString sourcedir;
};

#endif // FILESELECTOR_H

#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QProcess>
#include <QSettings>
#include <QMenu>

#include "ui_files.h"

class FileSelector : public QDockWidget
{
public:
	FileSelector(QWidget *parent);

	void refresh();

	void setSourceDir(QString path);

public slots:
	void customContextMenuRequested(const QPoint&);

private:
	Ui::Files *ui;

	QFileSystemModel *filesystem;
	QMenu *filesContext;
	QString sourcedir;
};

#endif // FILESELECTOR_H

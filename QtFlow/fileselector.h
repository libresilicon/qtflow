#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QProcess>
#include <QSettings>
#include <QMenu>

#include "qtflowfilelist.h"

#include "ui_files.h"

#include "project.h"

class FileSelector : public QDockWidget
{
	Q_OBJECT

public:
	FileSelector(QWidget *parent);

	void setProject(Project *p);

public slots:
	void onContextMenu(const QPoint&);
	void onOpen(const QModelIndex&);
	void refresh();

signals:
	void openFile(QString);

private:
	Ui::Files *ui;

	QtFlowFileList *filesystem;
	QMenu *context;

	Project *project;
};

#endif // FILESELECTOR_H

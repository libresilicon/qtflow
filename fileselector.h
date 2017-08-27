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
	Q_OBJECT

public:
	FileSelector(QWidget *parent);

	void refresh();

	void setSourceDir(QString path);

public slots:
	void onContextMenu(const QPoint&);
	void onOpen(const QModelIndex&);
	void onSetTestBenchFile(QString);
	void onSetTopLevelFile(QString);

signals:
	void openFile(QString);

private:
	Ui::Files *ui;

	QFileSystemModel *filesystem;
	QMenu *context;
	QString sourcedir;
};

#endif // FILESELECTOR_H

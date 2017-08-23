#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QDockWidget>
#include <QSettings>

#include "ui_files.h"

class FileSelector : public QDockWidget
{
public:
	FileSelector(QWidget *parent);

private:
	Ui::Files *ui;
};

#endif // FILESELECTOR_H

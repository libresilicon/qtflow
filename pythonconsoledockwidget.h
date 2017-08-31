#ifndef PYTHONCONSOLEDOCKWIDGET_H
#define PYTHONCONSOLEDOCKWIDGET_H

#include <QDockWidget>
#include "ui_console.h"

class PythonConsoleDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit PythonConsoleDockWidget(QWidget *parent = nullptr);
	
signals:
	
public slots:
private:
	Ui::PythonConsole *ui;
};

#endif // PYTHONCONSOLEDOCKWIDGET_H

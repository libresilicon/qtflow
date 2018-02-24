#ifndef PYTHONCONSOLEDOCKWIDGET_H
#define PYTHONCONSOLEDOCKWIDGET_H

#include <QDockWidget>
#include "ui_console.h"
#include "gui/PythonQtScriptingConsole.h"

class PythonConsoleDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit PythonConsoleDockWidget(QWidget *parent = nullptr, PythonQtObjectPtr context = nullptr);

signals:

public slots:
private:
	PythonQtScriptingConsole *console;
	PythonQtObjectPtr mainContext;
	Ui::PythonConsole *ui;
};

#endif // PYTHONCONSOLEDOCKWIDGET_H

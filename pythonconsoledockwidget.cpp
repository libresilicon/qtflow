#include "pythonconsoledockwidget.h"

PythonConsoleDockWidget::PythonConsoleDockWidget(QWidget *parent, PythonQtObjectPtr *context) :
	QDockWidget(parent),
	ui(new Ui::PythonConsole),
	mainContext(context)
{
	ui->setupUi(this);
	console = new PythonQtScriptingConsole(ui->dockWidgetContents, *mainContext);
	ui->dockWidgetContents->layout()->addWidget(console);
	console->showMaximized();
}

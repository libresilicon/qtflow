#include "pythonconsoledockwidget.h"

PythonConsoleDockWidget::PythonConsoleDockWidget(QWidget *parent, PythonQtObjectPtr *context) :
	QDockWidget(parent),
	ui(new Ui::PythonConsole),
	mainContext(context)
{
	ui->setupUi(this);
	console = new NicePyConsole(ui->pythonConsole, mainContext);
	console->showMaximized();
}

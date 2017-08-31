#include "pythonconsoledockwidget.h"

PythonConsoleDockWidget::PythonConsoleDockWidget(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::PythonConsole)
{
	ui->setupUi(this);
}

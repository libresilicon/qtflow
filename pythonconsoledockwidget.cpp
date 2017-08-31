#include "pythonconsoledockwidget.h"

PythonConsoleDockWidget::PythonConsoleDockWidget(QWidget *parent) :
	QDockWidget(parent),
	consoleWidget(new PythonConsoleWidget(this)),
	layout(new QVBoxLayout(this))
{
	setLayout(layout);
	layout->addWidget(consoleWidget);
}

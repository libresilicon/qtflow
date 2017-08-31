#include "pythonconsolewidget.h"

PythonConsoleWidget::PythonConsoleWidget(QWidget *parent) :
	QWidget(parent),
	mainPythonContext(NULL),
	consoleWindow(NULL),
	layout(new QVBoxLayout(this))
{
	mainPythonContext = PythonQt::self()->getMainModule();
	consoleWindow = new NicePyConsole(this, mainPythonContext);
	layout->addWidget(consoleWindow);
	setLayout(layout);
}

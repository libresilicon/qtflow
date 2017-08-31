#ifndef PYTHONCONSOLEWIDGET_H
#define PYTHONCONSOLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "NicePyConsole.h"

class PythonConsoleWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PythonConsoleWidget(QWidget *parent = nullptr);
	
signals:
	
public slots:
private:
	PythonQtObjectPtr mainPythonContext;
	NicePyConsole* consoleWindow;
	QVBoxLayout *layout;
};

#endif // PYTHONCONSOLEWIDGET_H

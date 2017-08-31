#ifndef PYTHONCONSOLEDOCKWIDGET_H
#define PYTHONCONSOLEDOCKWIDGET_H

#include <QDockWidget>
#include "pythonconsolewidget.h"

class PythonConsoleDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit PythonConsoleDockWidget(QWidget *parent = nullptr);
	
signals:
	
public slots:
private:
	PythonConsoleWidget *consoleWidget;
	QVBoxLayout *layout;
};

#endif // PYTHONCONSOLEDOCKWIDGET_H

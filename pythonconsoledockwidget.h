#ifndef PYTHONCONSOLEDOCKWIDGET_H
#define PYTHONCONSOLEDOCKWIDGET_H

#include <QObject>
#include <QWidget>

class PythonConsoleDockWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PythonConsoleDockWidget(QWidget *parent = nullptr);
  
signals:
  
public slots:
};

#endif // PYTHONCONSOLEDOCKWIDGET_H
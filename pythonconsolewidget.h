#ifndef PYTHONCONSOLEWIDGET_H
#define PYTHONCONSOLEWIDGET_H

#include <QObject>
#include <QWidget>

class PythonConsoleWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PythonConsoleWidget(QWidget *parent = nullptr);
  
signals:
  
public slots:
};

#endif // PYTHONCONSOLEWIDGET_H
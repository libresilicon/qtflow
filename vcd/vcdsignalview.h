#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QObject>
#include <QWidget>

class VcdSignalView : public QWidget
{
  Q_OBJECT
public:
  explicit VcdSignalView(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // VCDSIGNALVIEW_H
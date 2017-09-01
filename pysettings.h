#ifndef PYSETTINGS_H
#define PYSETTINGS_H

#include <QObject>

class PySettings : public QObject
{
  Q_OBJECT
public:
  explicit PySettings(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PYSETTINGS_H
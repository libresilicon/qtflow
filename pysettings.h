#ifndef PYSETTINGS_H
#define PYSETTINGS_H

#include "PythonQt.h"

#include <QObject>
#include <QStringList>
#include <QMainWindow>
#include <QPushButton>
#include <QVariant>
#include <QLayout>

class PySettings : public QObject
{
	Q_OBJECT
public:
	explicit PySettings(QObject *parent = nullptr);

public Q_SLOTS:
	PyObject* getMainModule();
	void test();
};

#endif // PYSETTINGS_H

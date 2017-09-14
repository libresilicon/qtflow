#ifndef PYSETTINGS_H
#define PYSETTINGS_H

#include "PythonQt.h"

#include <QObject>
#include <QStringList>
#include <QMainWindow>
#include <QPushButton>
#include <QVariant>
#include <QLayout>
#include <QSettings>

class PySettings : public QObject
{
	Q_OBJECT
public:
	explicit PySettings(QObject *parent = nullptr, QSettings *s = nullptr);

public Q_SLOTS:
	PyObject* getMainModule();

	QString getIcarus();
	QString getYosys();
	QString getGrayWolf();
	QString getQRouter();

private:
	QSettings *settings;
};

#endif // PYSETTINGS_H

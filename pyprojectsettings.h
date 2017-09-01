#ifndef PYPROJECTSETTINGS_H
#define PYPROJECTSETTINGS_H

#include "PythonQt.h"

#include <QObject>
#include <QStringList>
#include <QMainWindow>
#include <QPushButton>
#include <QVariant>
#include <QLayout>
#include <QSettings>

class PyProjectSettings : public QObject
{
	Q_OBJECT

public:
	explicit PyProjectSettings(QObject *parent = nullptr, QSettings *s = nullptr);

public Q_SLOTS:
	PyObject* getMainModule();

	QString getTopLevel();

	QString getRootDir();
	QString getLayoutDir();
	QString getSourceDir();
	QString getSynthesisDir();
	QString getTestBench();

private:
	QSettings *settings;
};

#endif // PYPROJECTSETTINGS_H

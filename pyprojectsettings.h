#ifndef PYPROJECTSETTINGS_H
#define PYPROJECTSETTINGS_H

#include "PythonQt.h"

#include "project.h"

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
	explicit PyProjectSettings(Project *parent = nullptr);

public Q_SLOTS:
	PyObject* getMainModule();

	QString getTopLevel();
	QString getTestBench();

	QString getRootDir();
	QString getLayoutDir();
	QString getSourceDir();
	QString getSynthesisDir();

	QString getVCDFile();
	QString getVCDPath();

private:
	Project *project;
};

#endif // PYPROJECTSETTINGS_H

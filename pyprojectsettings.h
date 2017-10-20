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

	QString getTopLevelFile();
	QString getTestBenchFile();

	QString getRootDir();
	QString getLayoutDir();
	QString getSourceDir();
	QString getSynthesisDir();

	QStringList getSearchDirectories();

	QString getParametersFile();

	// synthesis files:
	QString getLibertyFile();

	QString getVCDFile();
	QString getVCDPath();

	// synthesis options:
	bool hasSettingABC();

	int getMacroValue(QString macro_name, QString value_name);
	int getMacroPinValue(QString macro_name, QString pin_name, QString value_name);
	int getBaseUnits(QString macro_name);
	int getDistanceUnits();

private:
	Project *project;
};

#endif // PYPROJECTSETTINGS_H

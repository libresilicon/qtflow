#include "pyprojectsettings.h"

PyProjectSettings::PyProjectSettings(QObject *parent, QSettings *s) :
	QObject(parent),
	settings(s)
{
}

PyObject* PyProjectSettings::getMainModule()
{
		return PythonQt::self()->getMainModule();
}

QString PyProjectSettings::getTopLevel()
{
	return settings->value("toplevel").toString();
}

QString PyProjectSettings::getRootDir()
{
	return settings->value("rootdir").toString();
}

QString PyProjectSettings::getLayoutDir()
{
	return settings->value("layout").toString();
}

QString PyProjectSettings::getSourceDir()
{
	return settings->value("sourcedir").toString();
}

QString PyProjectSettings::getSynthesisDir()
{
	return settings->value("synthesis").toString();
}

QString PyProjectSettings::getTestBench()
{
	return settings->value("testbench").toString();
}

#include "pysettings.h"

PySettings::PySettings(QObject *parent, QSettings *s) :
	QObject(parent),
	settings(s)
{
}

QString PySettings::getIcarus()
{
	QString r;
	if(settings) r = settings->value("icarus").toString();
	return r;
}

QString PySettings::getYosys()
{
	QString r;
	if(settings) r = settings->value("yosys").toString();
	return r;
}

QString PySettings::getGrayWolf()
{
	QString r;
	if(settings) r = settings->value("graywolf").toString();
	return r;
}

QString PySettings::getQRouter()
{
	QString r;
	if(settings) r = settings->value("qrouter").toString();
	return r;
}

PyObject* PySettings::getMainModule()
{
        return PythonQt::self()->getMainModule();
}

#include "project.h"
#include "pyprojectsettings.h"

PyProjectSettings::PyProjectSettings(Project *parent) :
	QObject(parent),
	project(parent)
{
}

PyObject* PyProjectSettings::getMainModule()
{
	return PythonQt::self()->getMainModule();
}

QString PyProjectSettings::getTopLevel()
{
	return project->getTopLevel();
}

QString PyProjectSettings::getRootDir()
{
	return project->getRootDir();
}

QString PyProjectSettings::getLayoutDir()
{
	return project->getLayoutDir();
}

QString PyProjectSettings::getSourceDir()
{
	return project->getSourceDir();
}

QString PyProjectSettings::getSynthesisDir()
{
	return project->getSynthesisDir();
}

QString PyProjectSettings::getTestBench()
{
	return project->getTestBench();
}

QString PyProjectSettings::getVCDFile()
{
	return project->getVCDFile();
}

QString PyProjectSettings::getVCDPath()
{
	return project->getVCDPath();
}

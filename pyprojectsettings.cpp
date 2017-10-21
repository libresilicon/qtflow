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

QString PyProjectSettings::getParametersFile()
{
	return project->getParametersFile();
}

QString PyProjectSettings::getVCDPath()
{
	return project->getVCDPath();
}

QStringList PyProjectSettings::getSearchDirectories()
{
	return project->getSearchDirectories();
}

QString PyProjectSettings::getTestBenchFile()
{
	return project->getTestBenchFile();
}

QString PyProjectSettings::getTopLevelFile()
{
	return project->getTopLevelFile();
}

QString PyProjectSettings::getLibertyFile()
{
	return QFileInfo(project->getLibertyFile()).absoluteFilePath();
}

bool PyProjectSettings::hasSettingABC()
{
	return project->hasSettingABC();
}

// for building CEL data:
int PyProjectSettings::getMacroValue(QString macro_name, QString value_name)
{
	int ret = 1;
	int w,h,units;
	int left,right,bottom,top;
	lef::LEFMacro* macro;
	if(project) {
		if(project->isDefinedMacro(macro_name)) {
			macro = project->getMacro(macro_name);
			if(macro) {
				units = project->getBaseUnits(macro_name);
				w = (units*macro->getWidth());
				h = (units*macro->getHeight());
				left = -w/2;
				right = left+w;
				bottom = -h/2;
				top = bottom+h;
				if(value_name=="left") {
					ret = left;
				} else if(value_name=="right") {
					ret = right;
				} else if(value_name=="bottom") {
					ret = bottom;
				} else if(value_name=="top") {
					ret = top;
				}
			}
		} else {
			qDebug() << __FUNCTION__ << " unknown: " << macro_name << " " << value_name;
		}
	}
	return ret;
}

int PyProjectSettings::getMacroPinValue(QString macro_name, QString pin_name, QString value_name)
{
	int ret = 1;
	int w,h,units;
	int xp, yp;
	lef::LEFMacro* macro;
	lef::LEFPin* pin;
	QPointF center;
	if(project) {
		if(project->isDefinedMacro(macro_name)) {
			macro = project->getMacro(macro_name);
			if(macro) {
				units = project->getBaseUnits(macro_name);
				w = (units*macro->getWidth());
				h = (units*macro->getHeight());

				if(macro->pinExists(pin_name)) {
					pin = macro->getPin(pin_name);
					center = pin->getCenter();
					xp = center.x()*units-w/2;
					yp = center.y()*units-h/2;

					if(value_name=="xp")
						ret = xp;
					if(value_name=="yp")
						ret = yp;
				}
			}
		} else {
			qDebug() << __FUNCTION__ << " unknown: " << macro_name << " " << pin_name << " " << value_name;
		}
	}
	return ret;
}

int PyProjectSettings::getBaseUnits(QString macro_name)
{
	return project->getBaseUnits(macro_name);
}

int PyProjectSettings::getDistanceUnits()
{
	return project->getSmallestUnit();
}

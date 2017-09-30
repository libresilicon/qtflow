#include "project.h"

IProject::IProject() : QObject()
{
}

Project::Project(QSettings *s, QString path, PythonQtObjectPtr *main) :
	IProject(),
	settings(s),
	mainContext(main),
	settingsFileProcess(NULL),
	techdata(NULL),
	colorMap(new ColorMap())
{
	QTemporaryDir temporaryDir;
	QString filedest;

	if(QFile(path).exists()) {
		project_settings = new QSettings(path, QSettings::NativeFormat);
		if(project_settings->value("sourcedir","").toString()=="") {
			QTextStream(stdout) << "No variable called sourcedir set!!\n";
		}
		if(project_settings->value("projectType","").toString()=="") {
			QTextStream(stdout) << "No variable called projectType set!!\n\tSetting to default";
			project_settings->setValue("projectType","asic_mixed");
			project_settings->sync();
		}
	} else {
		create(path);
	}

	settings->beginGroup("history");
	QStringList recentProjectsList = settings->value("recentProjects").toStringList();
	recentProjectsList.append(path);
	recentProjectsList.removeDuplicates();
	settings->setValue("recentProjects",recentProjectsList);
	settings->endGroup();
	settings->sync();

	settingsFileProcess = new QDomDocument();
	QFile file(":/process.xml");
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	filedest = temporaryDir.path()+"/tech";
	QFile::copy(getTechnologyFile(), filedest);
	if(QFile(filedest).exists()) {
		qDebug() << "Opening here: " << filedest;
		techdata = new tech::TechData(filedest);
	}

	// setting up color table:
	filedest = temporaryDir.path()+"/color";
	QFile::copy(getColorMapFile(), filedest);
	if(QFile(filedest).exists()) {
		qDebug() << "Opening here: " << filedest;
		colorMap->loadColors(filedest);
	}

	// setting up color object match table:
	filedest = temporaryDir.path()+"/style";
	QFile::copy(getDesignStyleFile(), filedest);
	if(QFile(filedest).exists()) {
		qDebug() << "Opening here: " << filedest;
		colorMap->loadDesign(filedest);
	}

}

Project::~Project()
{
	delete colorMap;
	delete techdata;
	delete project_settings;
}

QString Project::getSourceDir()
{
	return project_settings->value("sourcedir").toString();
}

QString Project::getSynthesisDir()
{
	return project_settings->value("synthesis").toString();
}

QString Project::getRootDir()
{
	return project_settings->value("rootdir").toString();
}

QString Project::getTopLevel()
{
	return project_settings->value("toplevel").toString();
}

QString Project::getTestBench()
{
	return project_settings->value("testbench").toString();
}

QString Project::getLayoutDir()
{
	return project_settings->value("layout").toString();
}

QString Project::getVCDFile()
{
	return this->getTestBench()+".vcd";
}

QString Project::getVCDPath()
{
	return this->getSynthesisDir()+'/'+this->getVCDFile();
}

QString Project::getTechnology()
{
	return project_settings->value("technology").toString();
}

QString Project::getProcess()
{
	return project_settings->value("process").toString();
}

QStringList Project::getProcessFiles()
{
	QStringList ret;
	
	QString technology = getTechnology();
	QString process = getProcess();

	QDomElement e1, e2, e3, e4;

	QDomNodeList nl1, nl2, nl3, nl4;

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(e1.attribute("xml:id")==technology) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					if(e2.attribute("xml:id")==process) {
						nl3 = e2.childNodes();
						for(int k = 0; k < nl3.count(); k++) {
							e3 = nl3.at(k).toElement();
							if(e3.tagName()=="lef") {
								nl4 = e3.childNodes();
								for(int l = 0; l < nl4.count(); l++) {
									e4 = nl4.at(l).toElement();
									if(e4.tagName()=="file") {
										ret.append(e4.text());
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}


QString Project::getTechnologyFile()
{
	QString ret;

	QString technology = getTechnology();
	QString process = getProcess();

	QDomElement e1, e2, e3;

	QDomNodeList nl1, nl2, nl3;

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(e1.attribute("xml:id")==technology) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					if(e2.attribute("xml:id")==process) {
						nl3 = e2.childNodes();
						for(int k = 0; k < nl3.count(); k++) {
							e3 = nl3.at(k).toElement();
							if(e3.tagName()=="tech") {
								ret=e3.text();
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

QString Project::getDesignStyleFile()
{
	QString ret = ":/mos.dstyle";

	QString technology = getTechnology();
	QString process = getProcess();

	QDomElement e1, e2, e3;

	QDomNodeList nl1, nl2, nl3;

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(e1.attribute("xml:id")==technology) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					if(e2.attribute("xml:id")==process) {
						nl3 = e2.childNodes();
						for(int k = 0; k < nl3.count(); k++) {
							e3 = nl3.at(k).toElement();
							if(e3.tagName()=="design") {
								ret=e3.text();
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

QString Project::getColorMapFile()
{
	QString ret = ":/mos.cmap";

	QString technology = getTechnology();
	QString process = getProcess();

	QDomElement e1, e2, e3;

	QDomNodeList nl1, nl2, nl3;

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(e1.attribute("xml:id")==technology) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					if(e2.attribute("xml:id")==process) {
						nl3 = e2.childNodes();
						for(int k = 0; k < nl3.count(); k++) {
							e3 = nl3.at(k).toElement();
							if(e3.tagName()=="colors") {
								ret=e3.text();
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

QString Project::getProjectType()
{
	return project_settings->value("projectType").toString();
}

void Project::setTestBench(QString top)
{
	project_settings->setValue("testbench",top);
	project_settings->sync();
}

void Project::setTopLevel(QString top)
{
	project_settings->setValue("toplevel",top);
	project_settings->sync();
}

void Project::setTechnology(QString tech)
{
	project_settings->setValue("technology",tech);
	project_settings->sync();
}

void Project::setProcess(QString proc)
{
	project_settings->setValue("process",proc);
	project_settings->sync();
}

void Project::createFiles()
{
	QStringList folders;
	QString magicLayout;

	folders << getSourceDir();
	folders << getLayoutDir();
	folders << getSynthesisDir();

	QDir folder;
	foreach(QString f, folders) {
		folder = QDir(f);
		if(!folder.exists()) folder.mkdir(f);
	}

	magicLayout = getLayoutDir();
	magicLayout += '/';
	magicLayout += getTopLevel();
	magicLayout += ".mag";

	QFile magicFile(magicLayout);
	if(magicFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&magicFile);
		outputStream << "magic" << endl;
		outputStream << "tech " << getTechnology() << endl;
		outputStream << "magscale 1 2" << endl;
		outputStream << "timestamp " << QDateTime::currentMSecsSinceEpoch() << endl;
		outputStream << "<< end >>" << endl;
		magicFile.close();
	}
}

void Project::setProjectType(QString proc)
{
	project_settings->setValue("projectType",proc);
	project_settings->sync();
}

void Project::create(QString path)
{
	QString rootdir;

	project_settings = new QSettings(path, QSettings::NativeFormat);
	project_settings->setValue("technology", "osu035");
	project_settings->sync();

	rootdir = QFileInfo(project_settings->fileName()).absolutePath();
	project_settings->setValue("rootdir", rootdir);
	project_settings->setValue("sourcedir", rootdir+"/source");
	project_settings->setValue("synthesis", rootdir+"/synthesis");
	project_settings->setValue("layout", rootdir+"/layout");
	project_settings->sync();
}

void Project::synthesis()
{
	mainContext->evalFile(":/scripts/synthesis.py");
}

void Project::simulation()
{
	mainContext->evalFile(":/scripts/simulation.py");
}

void Project::placement()
{
}

void Project::routing()
{
}

void Project::buildAll()
{
}

QColor Project::colorMat(QString material)
{
	return colorMap->fromName(material);
}

qreal Project::posMat(QString material)
{
	// TODO:
	// make this configuration based!
	// don't hardcode this!
	qreal ret = 0;

	if (material == "metal1")
		ret = 0.0;
	if (material == "metal2")
		ret = 0.1;
	if (material == "metal3")
		ret = 0.2;
	if (material == "metal4")
		ret = 0.3;
	if (material == "m1contact")
		ret = 0.4;
	if (material == "m2contact")
		ret = 0.5;
	if (material == "m3contact")
		ret = 0.6;
	if (material == "m4contact")
		ret = 0.7;

	return ret;
}

qreal Project::thicknessMat(QString material)
{
	// TODO:
	// make this configuration based!
	// don't hardcode this!
	qreal ret = 0.05;

	return ret;
}


QStringList Project::getPlanes()
{
	QStringList planeList;
	if(!techdata) return planeList;

	QStringList tmpstrarr;
	foreach(QString s, techdata->getPlanes()) {
		tmpstrarr = s.split(',');
		planeList.append(tmpstrarr.at(0));
	}

	return planeList;
}

QStringList Project::getTypeNames()
{
	if(!techdata) return QStringList();
	return techdata->getTypeNames();
}

QStringList Project::getType(QString s)
{
	QStringList typeList;
	if(!techdata) return typeList;

	QStringList tmpstrarr;
	QString name;
	foreach(QString t, techdata->getType(s)) {
		tmpstrarr = t.split(',');
		name = tmpstrarr.at(0);
		if(!typeList.contains(name))
			typeList.append(name);
	}

	return typeList;
}

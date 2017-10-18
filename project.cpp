#include "project.h"

IProject::IProject() : QObject()
{
}

Project::Project(QSettings *s, QString path, PythonQtObjectPtr *main) :
	IProject(),
	settings(s),
	mainContext(main),
	settingsFileProcess(NULL),
	techDisplayData(NULL),
	colorMap(new ColorMap())
{
	QTemporaryDir temporaryDir;
	QString filedest;

	if(QFile(path).exists()) {
		project_settings = new QSettings(path, QSettings::NativeFormat);
		rootdir = QFileInfo(project_settings->fileName()).absolutePath();
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
	QFile file(QDir(getTechPath()).filePath("process.xml"));
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	filedest = QDir(temporaryDir.path()).filePath("tech");
	QFile::copy(getTechnologyDisplayFile(), filedest);
	if(QFile(filedest).exists()) {
		techDisplayData = new tech::TechData(filedest);
	}

	// setting up color table:
	filedest = QDir(temporaryDir.path()).filePath("color");
	QFile::copy(getColorMapFile(), filedest);
	if(QFile(filedest).exists()) {
		colorMap->loadColors(filedest);
	}

	// setting up color object match table:
	filedest = QDir(temporaryDir.path()).filePath("style");
	QFile::copy(getDesignStyleFile(), filedest);
	if(QFile(filedest).exists()) {
		colorMap->loadDesign(filedest);
	}

	loadLibraryFiles();
	loadSchematicsLibraryFiles();
}

Project::~Project()
{
	delete colorMap;
	delete techDisplayData;
	delete project_settings;
}

QStringList Project::getSearchDirectories()
{
	QStringList ret = project_settings->value("search_dirs").toStringList();
	if(ret.count()==0)
		ret << getSourceDir();
	return ret;
}

void Project::setSearchDirectories(QStringList l)
{
	project_settings->setValue("search_dirs",l);
	project_settings->sync();
}

QString Project::getSourceDir()
{
	return QDir(rootdir).filePath(project_settings->value("sourcedir").toString());
}

QString Project::getSynthesisDir()
{
	return QDir(rootdir).filePath(project_settings->value("synthesis").toString());
}

QString Project::getLayoutDir()
{
	return QDir(rootdir).filePath(project_settings->value("layout").toString());
}

QString Project::getRootDir()
{
	return rootdir;
}

QString Project::getTopLevelFile()
{
	return QDir(getSourceDir()).filePath(project_settings->value("toplevel").toString()+".v");
}

QString Project::getTestBenchFile()
{
	return QDir(getSourceDir()).filePath(project_settings->value("testbench").toString()+".v");
}

QString Project::getTopLevel()
{
	return project_settings->value("toplevel").toString();
}

QString Project::getTestBench()
{
	return project_settings->value("testbench").toString();
}

QString Project::getVCDFile()
{
	return this->getTestBench()+".vcd";
}

QString Project::getVCDPath()
{
	return QDir(this->getSynthesisDir()).filePath(this->getVCDFile());
}

QString Project::getTechnology()
{
	return project_settings->value("technology").toString();
}

QString Project::getProcess()
{
	return project_settings->value("process").toString();
}

QStringList Project::getLibraryFiles()
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
										ret.append(QDir(getTechPath()).filePath(e4.text()));
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

QStringList Project::getSchematicsLibraryNames()
{
	QStringList ret;
	foreach(QString filename, getSchematicsLibraryFiles()) {
		ret << QFileInfo(filename).baseName();
	}
	return ret;
}

QStringList Project::getSchematicsLibraryParts(QString s)
{
	if(slibdata.contains(s)) {
		return slibdata[s]->getSymbolNames();
	}
	return QStringList();
}

QStringList Project::getSchematicsLibraryFiles()
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
							if(e3.tagName()=="symbols") {
								nl4 = e3.childNodes();
								for(int l = 0; l < nl4.count(); l++) {
									e4 = nl4.at(l).toElement();
									if(e4.tagName()=="file") {
										ret.append(QDir(getTechPath()).filePath(e4.text()));
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

QString Project::getTechnologyDisplayFile()
{
	QString ret = "scmos.tech";

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
							if(e3.tagName()=="techdisplay") {
								ret=e3.text();
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath(ret);
}

QString Project::getTechPath()
{
	QString ret;
	ret = settings->value("tech_path").toString();
	if(ret==QString()) ret = ":/";
	return ret;
}

QString Project::getTechnologyFile()
{
	QString ret = "scmos.tech";

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

	return QDir(getTechPath()).filePath(ret);
}

QString Project::getDesignStyleFile()
{
	QString ret = "mos.dstyle";

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

	return QDir(getTechPath()).filePath(ret);
}

QString Project::getColorMapFile()
{
	QString ret = "mos.cmap";

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

	return QDir(getTechPath()).filePath(ret);
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

	magicLayout = QDir(getLayoutDir()).filePath(getTopLevel()+".mag");

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
	project_settings = new QSettings(path, QSettings::NativeFormat);
	rootdir = QFileInfo(project_settings->fileName()).absolutePath();

	project_settings->setValue("technology", "osu035");
	project_settings->sync();
	project_settings->setValue("sourcedir", QDir(rootdir).filePath("source"));
	project_settings->setValue("synthesis", QDir(rootdir).filePath("synthesis"));
	project_settings->setValue("layout", QDir(rootdir).filePath("layout"));
	project_settings->sync();
}

void Project::synthesis()
{

	if(QFile(getSynthesisScript()).exists()) {
		mainContext->evalFile(getSynthesisScript());
	}
}

void Project::simulation()
{
	if(QFile(getSimulationScript()).exists()) {
		mainContext->evalFile(getSimulationScript());
		emit(simulationDone());
	}
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

bool Project::hasMaterialTypeMapping(QString material)
{
	QMap<QString,QVector<int>> l;
	foreach(QString s, techDisplayData->getStyleNames()) {
		l = techDisplayData->getStyle(s);
		foreach(QString o, l.keys()) {
			if(o==material) {
				return true;
			}
		}
	}
	return false;
}

QColor Project::colorMaterialTypeMapping(QString material)
{
	QMap<QString,QVector<int>> l;
	foreach(QString s, techDisplayData->getStyleNames()) {
		l = techDisplayData->getStyle(s);
		foreach(QString o, l.keys()) {
			if(o==material) {
				if(l[o].count()>0) {
					return colorMap->colorFromCode(l[o].at(0));
				}
			}
		}
	}
	return QColor(Qt::white);
}

QColor Project::colorMat(QString material)
{
	QMap<QString,QVector<int>> l;
	if(techDisplayData) {
		if(colorMap->colorNameExists(material)) {
			return colorMap->colorFromName(material);
		} else {
			foreach(QString altname, getAlternativeNames(material)) {
				if(colorMap->colorNameExists(altname)) {
					return colorMap->colorFromName(altname);
				} else if(hasMaterialTypeMapping(altname)) {
					return colorMaterialTypeMapping(altname);
				}
			}
		}
	}
	return QColor(Qt::white);
}

QIcon Project::materialIcon(QString material)
{
	QPixmap pm;
	QIcon ico;

	//if(colorMap->isStipple(material)) {
	//	pm = colorMap->getStipplePixMap(material);
	//} else {
		pm = QPixmap(100,100);
		pm.fill(colorMat(material));
	//}
	ico = QIcon(pm);

	return ico;
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
	if(!techDisplayData) return planeList;

	QStringList tmpstrarr;
	foreach(QString s, techDisplayData->getPlanes()) {
		tmpstrarr = s.split(',');
		planeList.append(tmpstrarr.at(0));
	}

	return planeList;
}

QStringList Project::getTypeNames()
{
	if(!techDisplayData) return QStringList();
	return techDisplayData->getTypeNames();
}

QStringList Project::getType(QString s)
{
	QStringList typeList;
	if(!techDisplayData) return typeList;

	QStringList tmpstrarr;
	QString name;
	foreach(QString t, techDisplayData->getType(s)) {
		tmpstrarr = t.split(',');
		name = tmpstrarr.at(0);
		if(!typeList.contains(name))
			typeList.append(name);
	}

	return typeList;
}

QStringList Project::getAlternativeNames(QString s)
{
	if(!techDisplayData) return QStringList();

	QStringList tmpstrarr;

	QStringList type;
	foreach(QString tn, techDisplayData->getTypeNames()) {
		type = techDisplayData->getType(tn);
		foreach(QString t, type) {
			tmpstrarr = t.split(',');
			foreach(QString name, tmpstrarr) {
				if(name==s) {
					return tmpstrarr;
				}
			}
		}
	}

	return QStringList();
}

bool Project::hasSettingABC()
{
	bool r;
	project_settings->beginGroup("synthesis");
	r = project_settings->value("runABC",false).toBool();
	project_settings->endGroup();
	return r;
}

void Project::setSettingABC(bool t)
{
	project_settings->beginGroup("synthesis");
	project_settings->setValue("runABC",t);
	project_settings->endGroup();
	project_settings->sync();
}

// LEF operations:
bool Project::isDefinedMacro(QString s)
{
	foreach(QString key, lefdata.keys()) {
		if(lefdata[key]->isDefinedMacro(s))
			return true;
	}
	return false;
}

lef::LEFMacro* Project::getMacro(QString s)
{
	foreach(QString key, lefdata.keys()) {
		if(lefdata[key]->isDefinedMacro(s)) {
			return lefdata[key]->getMacro(s);
		}
	}
	return NULL;
}

void Project::loadLibraryFiles()
{
	QTemporaryDir temporaryDir;
	QString filedest;
	QString libname;

	foreach(QString filename, getLibraryFiles()) {
		filedest = QDir(temporaryDir.path()).filePath("cells.lef");
		QFile::copy(filename, filedest);
		if(QFile(filedest).exists()) {
				libname = QFileInfo(filename).baseName();
				lefdata[libname] = new lef::LEFData(filedest);
		}
	}
}

// Schematics operations:
bool  Project::isDefinedPart(QString s)
{
	foreach(QString key, slibdata.keys()) {
		if(slibdata[key]->isDefinedSymbol(s))
			return true;
	}
	return false;
}

symbol::SchematicsSymbol* Project::getSchematicsPart(QString s)
{
	foreach(QString key, slibdata.keys()) {
		if(slibdata[key]->isDefinedSymbol(s))
			return slibdata[key]->getSymbol(s);
	}
	return NULL;
}

QStringList Project::getListOfSchematicParts()
{
	QStringList ret;
	foreach(QString key, slibdata.keys()) {
		ret+=slibdata[key]->getSymbolNames();
	}
	return ret;
}

void Project::loadSchematicsLibraryFiles()
{
	QTemporaryDir temporaryDir;
	QString filedest;
	QString libname;

	foreach(QString filename, getSchematicsLibraryFiles()) {
		filedest = QDir(temporaryDir.path()).filePath("cells.slib");
		QFile::copy(filename, filedest);
		if(QFile(filedest).exists()) {
			libname = QFileInfo(filename).baseName();
			slibdata[libname] = new symbol::SymbolData(filedest);
		}
	}
}

// python script paths:
QString Project::getSimulationScript()
{
	return project_settings->value("simulation_script",":/simulation.py").toString();
}

QString Project::getSynthesisScript()
{
	return project_settings->value("synthesis_script",":/synthesis.py").toString();
}

QString Project::getPlacementScript()
{

}

QString Project::getRoutingScript()
{

}

void Project::setSimulationScript(QString s)
{
	project_settings->setValue("simulation_script", s);
	project_settings->sync();
}

void Project::setSynthesisScript(QString s)
{
	project_settings->setValue("synthesis_script", s);
	project_settings->sync();
}

void Project::setPlacementScript(QString)
{

}

void Project::setRoutingScript(QString)
{

}

QString Project::getLibertyFile()
{
	QString ret = "osu035/osu035_stdcells.lib";

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
							if(e3.tagName()=="liberty") {
								ret=e3.text();
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath(ret);
}

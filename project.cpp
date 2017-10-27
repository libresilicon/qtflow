#include "project.h"

IProject::IProject() : QObject()
{
}

Project::Project(QSettings *s, QString path, PythonQtObjectPtr main) :
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
	loadScriptFiles();
	loadGDSFiles();
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
	if(ret.count()==0) {
		ret << getSourceDir();
		ret << getLayoutDir();
		ret << getSynthesisDir();
	}
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
										ret.append(QDir(getProcessPath()).filePath(e4.text()));
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

QStringList Project::getGDSFiles()
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
							if(e3.tagName()=="gds") {
								nl4 = e3.childNodes();
								for(int l = 0; l < nl4.count(); l++) {
									e4 = nl4.at(l).toElement();
									if(e4.tagName()=="file") {
										ret.append(QDir(getProcessPath()).filePath(e4.text()));
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
										ret.append(QDir(getProcessPath()).filePath(e4.text()));
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
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("scmos.tech");
}

QString Project::getTechPath()
{
	QString ret;
	ret = settings->value("tech_path").toString();
	if(ret==QString()) {
		qDebug() << "No technology search path set!";
		exit(0);
	}
	return ret;
}

QString Project::getProcessPath()
{
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
							if(e3.tagName()=="folder") {
								return QDir(getTechPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("osu035"); // default technology
}

QString Project::getParametersFile()
{
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
							if(e3.tagName()=="parameter") {
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("scmos.par"); // default technology;
}

QString Project::getTechnologyFile()
{
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
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("scmos.tech"); // default technology;
}

QString Project::getDesignStyleFile()
{
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
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("mos.dstyle");
}

QString Project::getColorMapFile()
{
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
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(getTechPath()).filePath("mos.cmap");
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

	folders << getSourceDir();
	folders << getLayoutDir();
	folders << getSynthesisDir();

	QDir folder;
	foreach(QString f, folders) {
		folder = QDir(f);
		if(!folder.exists()) folder.mkdir(f);
	}
}

void Project::setProjectType(QString proc)
{
	project_settings->setValue("projectType",proc);
	project_settings->sync();
}

bool Project::isMacroCellProject()
{
	QString v = project_settings->value("projectType").toString();
	return (v=="macro_cell");
}

void Project::setSyncLiberty(QString s)
{
	project_settings->setValue("syncLiberty",s);
	project_settings->sync();
}

void Project::setSyncLEF(QString s)
{
	project_settings->setValue("syncLEF",s);
	project_settings->sync();
}

void Project::setSyncSymbols(QString s)
{
	project_settings->setValue("syncSymbols",s);
	project_settings->sync();
}

QString Project::getSyncLiberty()
{
	return project_settings->value("syncLiberty").toString();
}

QString Project::getSyncLEF()
{
	return project_settings->value("syncLEF").toString();
}

QString Project::getSyncSymbols()
{
	return project_settings->value("syncSymbols").toString();
}

void Project::create(QString path)
{
	project_settings = new QSettings(path, QSettings::NativeFormat);
	rootdir = QFileInfo(project_settings->fileName()).absolutePath();

	project_settings->setValue("technology", "osu035");
	project_settings->sync();
	project_settings->setValue("sourcedir", "source");
	project_settings->setValue("synthesis", "synthesis");
	project_settings->setValue("layout", "layout");
	project_settings->sync();
}

void Project::synthesis()
{
	mainContext.evalScript("synth()");
}

void Project::simulation()
{
	mainContext.evalScript("sim()");
	emit(simulationDone());
}

void Project::placement()
{
}

void Project::routing()
{
}

void Project::buildAll()
{
	mainContext.evalScript("synth()");
	mainContext.evalScript("blif2cel()");
	mainContext.evalScript("blif2sym()");
	mainContext.evalScript("place()");
	mainContext.evalScript("place2def()");
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
	qDebug() << "Unable to map color to " << material;
	return QColor(Qt::black);
}

QColor Project::colorFromCode(int i)
{
	return colorMap->colorFromCode(i);
}

QString Project::getLongName(QString m)
{
	QString typeName;
	QStringList typeList;
	foreach(typeName, getTypeNames()) {
		foreach(typeList, getTypesMap(typeName)) {
			if(typeList.contains(m)) {
				return typeList[0];
			}
		}
	}
	return m;
}

QString Project::layerNameFromDStyle(int i)
{
	return getLongName(colorMap->materialNameFromCode(i));
}

QString Project::layerNameFromCIF(int i)
{
	return getLongName(techDisplayData->layerNameFromCIF(i));
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
					//qDebug() << "Found material " << material << " as " << altname;
					return colorMap->colorFromName(altname);
				} else if(hasMaterialTypeMapping(altname)) {
					//qDebug() << "Found material " << material << " as " << altname;
					return colorMaterialTypeMapping(altname);
				}
			}
		}
	}
	return QColor(Qt::black);
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

QString Project::material2Plane(QString m)
{
	QString plane;
	QString material;
	foreach(plane, getPlanes()) {
		foreach(material, getTypes(plane)) {
			if(material==m)
				return plane;
		}
	}
	return QString();
}

qreal Project::posMat(QString material)
{
	qreal ret = 0;
	QString plane;

	plane = material2Plane(material);

	if(techDisplayData) ret = techDisplayData->getPlaneOrder(plane);
	ret *= 0.1;

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
	QStringList tmpstrarr;

	if(techDisplayData) foreach(QString s, techDisplayData->getPlanes()) {
		tmpstrarr = s.split(',');
		planeList.append(tmpstrarr.at(0));
	}

	return planeList;
}

QStringList Project::getTypeNames()
{
	if(techDisplayData)
		return techDisplayData->getTypeNames();
	else
		return QStringList();
}

QList<QStringList> Project::getTypesMap(QString s)
{
	QList<QStringList> ret;
	QStringList typeList;

	if(techDisplayData) foreach(QString t, techDisplayData->getType(s)) {
		typeList = t.split(',');
		ret.append(typeList);
	}

	return ret;
}

QStringList Project::getTypes(QString s)
{
	QStringList typeList;
	QStringList tmpstrarr;
	QString name;

	if(techDisplayData) foreach(QString t, techDisplayData->getType(s)) {
		tmpstrarr = t.split(',');
		name = tmpstrarr.at(0);
		if(!typeList.contains(name))
			typeList.append(name);
	}

	return typeList;
}

QStringList Project::getAlternativeNames(QString s)
{
	QStringList tmpstrarr;
	QStringList type;

	if(techDisplayData) foreach(QString tn, techDisplayData->getTypeNames()) {
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

// GDS operations:
bool Project::isDefinedGDSMacro(QString s)
{
	foreach(QString key, gdtdata.keys()) {
		if(gdtdata[key]->containsCell(s))
			return true;
	}
	return false;
}

GDSCell* Project::getGDSMacro(QString s)
{
	foreach(QString key, gdtdata.keys()) {
		if(gdtdata[key]->containsCell(s)) {
			return gdtdata[key]->getCell(s);
		}
	}
	return NULL;
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

int Project::getBaseUnits(QString macro_name)
{
	int ret = 1;
	foreach(QString key, lefdata.keys()) {
		if(lefdata[key]->isDefinedMacro(macro_name)) {
			ret = lefdata[key]->getBaseUnits();
			break;
		}
	}
	return ret;
}

int Project::getSmallestUnit()
{
	int ret = 0;
	int u;

	foreach(QString key, lefdata.keys()) { // get biggest value
		u = lefdata[key]->getBaseUnits();
		if(u>0) {
			if(u>ret) ret = u;
		}
	}

	foreach(QString key, lefdata.keys()) { // get smallest value >0
		u = lefdata[key]->getBaseUnits();
		if(u>0) {
			if(u<ret) ret = u;
		}
	}

	return ret;
}

void Project::loadLibraryFiles()
{
	QString libname;
	foreach(QString filename, getLibraryFiles()) {
		if(QFile(filename).exists()) {
				libname = QFileInfo(filename).baseName();
				lefdata[libname] = new lef::LEFData(filename);
		} else {
			qDebug() << __FUNCTION__ << ": File " << filename << " doesn't exist";
		}
	}
}

void Project::loadGDSFiles()
{
	QString libname;
	foreach(QString filename, getGDSFiles()) {
		if(QFile(filename).exists()) {
				libname = QFileInfo(filename).baseName();
				gdtdata[libname] = new GDTData(filename);
		} else {
			qDebug() << __FUNCTION__ << ": File " << filename << " doesn't exist";
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

void Project::loadScriptFiles()
{
	if(QFile(getSynthesisScript()).exists()) {
		mainContext.evalFile(getSynthesisScript());
	} else {
		mainContext.evalScript("def synth():\n\tprint \"not defined\"");
	}

	if(QFile(getSimulationScript()).exists()) {
		mainContext.evalFile(getSimulationScript());
	} else {
		mainContext.evalScript("def sim():\n\tprint \"not defined\"");
	}

	if(QFile(getBLIF2CELScript()).exists()) {
		mainContext.evalFile(getBLIF2CELScript());
	} else {
		mainContext.evalScript("def blif2cel():\n\tprint \"not defined\"");
	}

	if(QFile(getBLIF2SymbolScript()).exists()) {
		mainContext.evalFile(getBLIF2SymbolScript());
	} else {
		mainContext.evalScript("def blif2sym():\n\tprint \"not defined\"");
	}

	if(QFile(getPlacementScript()).exists()) {
		mainContext.evalFile(getPlacementScript());
	} else {
		mainContext.evalScript("def place():\n\tprint \"not defined\"");
	}

	if(QFile(getPlace2DEFScript()).exists()) {
		mainContext.evalFile(getPlace2DEFScript());
	} else {
		mainContext.evalScript("def place2def():\n\tprint \"not defined\"");
	}
}

void Project::loadSchematicsLibraryFiles()
{
	QString libname;

	foreach(QString filename, getSchematicsLibraryFiles()) {
		if(QFile(filename).exists()) {
			libname = QFileInfo(filename).baseName();
			slibdata[libname] = new symbol::SymbolData(filename);
		} else {
			qDebug() << __FUNCTION__ << ": File " << filename << " doesn't exist";
		}
	}
}

// python script paths:
QString Project::getSimulationScript()
{
	return project_settings->value("simulation_script").toString();
}

QString Project::getSynthesisScript()
{
	return project_settings->value("synthesis_script").toString();
}

QString  Project::getBLIF2CELScript()
{
	return project_settings->value("blif2cel_script").toString();
}

QString  Project::getBLIF2SymbolScript()
{
	return project_settings->value("blif2sym_script").toString();
}

QString  Project::getPlace2DEFScript()
{
	return project_settings->value("place2def_script").toString();
}

QString Project::getPlacementScript()
{
	return project_settings->value("placement_script").toString();
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
								return QDir(getProcessPath()).filePath(e3.text());
							}
						}
					}
				}
			}
		}
	}

	return QDir(QDir(getTechnology()).filePath("osu035")).filePath("osu035_stdcells.lib");
}

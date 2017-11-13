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
	loadGDSFiles();
}

QStringList Project::getDesignRules()
{
	return techDisplayData->getDesignRules();
}


TechDesignRule Project::getDesignRule(QString n)
{
	return techDisplayData->getDesignRule(n);
}

QString Project::getFillCell()
{
	return "FILL";
}

// pad frame info
QStringList Project::getPadCells()
{
	QStringList ret;
	foreach(QString k, lefdata.keys()) {
		foreach(lef::LEFMacro* m, lefdata[k]->getMacros()) {
			if(m->getClass()=="PAD") {
				ret << m->getName();
			}
		}
	}
	return ret;
}

QStringList Project::getIOCells()
{
	QStringList ret;
	foreach(QString k, lefdata.keys()) {
		foreach(lef::LEFMacro* m, lefdata[k]->getMacros()) {
			if(m->getSite()=="IO") {
				ret << m->getName();
			}
		}
	}
	return ret;
}

QStringList Project::getCornerCells()
{
	QStringList ret;
	foreach(QString k, lefdata.keys()) {
		foreach(lef::LEFMacro* m, lefdata[k]->getMacros()) {
			if((m->getSite()=="corner")||m->getSite()=="CORNER") {
				ret << m->getName();
			}
		}
	}
	return ret;
}

QString Project::getClampPadCell()
{
	return "PADFC";
}

QString Project::getOutPutPadCell()
{
	return "PADOUT";
}

QString Project::getInPutPadCell()
{
	return "PADINC";
}

QString Project::getInOutPadCell()
{
	return "PADINOUT";
}

QString Project::getNCPadCell()
{
	return "PADNC";
}

QString Project::getVDDPadCell()
{
	return "PADVDD";
}

QString Project::getGNDPadCell()
{
	return "PADGND";
}

// special nets
QStringList Project::getPowerNets()
{
	QStringList pars = project_settings->value("power_nets").toStringList();
	QStringList ret = twoLevelListFilter("specialnets","power");

	if(pars.count())
		return pars;
	return ret;
}

QStringList Project::getGroundNets()
{
	QStringList pars = project_settings->value("ground_nets").toStringList();
	QStringList ret = twoLevelListFilter("specialnets","ground");

	if(pars.count())
		return pars;
	return ret;
}

QStringList Project::getClockNets()
{
	QStringList pars = project_settings->value("clock_nets").toStringList();
	QStringList ret = twoLevelListFilter("specialnets","clock");

	if(pars.count())
		return pars;
	return ret;
}

QString Project::getSpecialNetLayer(QString s)
{
	QString l = project_settings->value("special_net_"+s+"_layer").toString();
	if(l==QString())
		l = getRoutingLayers().at(0);

	return l;
}

void Project::setPowerNets(QStringList l)
{
	project_settings->setValue("power_nets", l);
}

void Project::setGroundNets(QStringList l)
{
	project_settings->setValue("ground_nets", l);
}

void Project::setClockNets(QStringList l)
{
	project_settings->setValue("clock_nets", l);
}

void Project::setSpecialNetLayer(QString s,QString l)
{
	project_settings->setValue("special_net_"+s+"_layer",l);
}

// ---------------

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

QStringList Project::twoLevelListFilter(QString filter1, QString filter2)
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
							if(e3.tagName()==filter1) {
								nl4 = e3.childNodes();
								for(int l = 0; l < nl4.count(); l++) {
									e4 = nl4.at(l).toElement();
									if(e4.tagName()==filter2) {
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

QString Project::oneLevelFilter(QString filter)
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
							if(e3.tagName()==filter) {
								return e3.text();
							}
						}
					}
				}
			}
		}
	}

	return QString();
}

QStringList Project::getLibraryFiles()
{
	QStringList ret;
	foreach(QString s, twoLevelListFilter("lef","file")) {
		ret.append(QDir(getProcessPath()).filePath(s));
	}
	return ret;
}

QStringList Project::getGDSFiles()
{
	QStringList ret;
	foreach(QString s, twoLevelListFilter("gds","file")) {
		ret.append(QDir(getProcessPath()).filePath(s));
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
	foreach(QString s, twoLevelListFilter("symbols","file")) {
		ret.append(QDir(getProcessPath()).filePath(s));
	}
	return ret;
}

QString Project::getTechnologyDisplayFile()
{
	QString ret = oneLevelFilter("techdisplay");
	if(ret==QString())
		return QDir(getTechPath()).filePath("scmos.tech");
	return QDir(getProcessPath()).filePath(ret);
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
	QString ret = oneLevelFilter("folder");
	if(ret==QString())
		return QDir(getTechPath()).filePath("osu035"); // default technology
	return QDir(getTechPath()).filePath(ret);
}

QString Project::getParametersFile()
{
	QString ret = oneLevelFilter("parameter");
	if(ret==QString())
		return QDir(getTechPath()).filePath("scmos.par"); // default technology;
	return QDir(getProcessPath()).filePath(ret);
}

QString Project::getTechnologyFile()
{
	QString ret = oneLevelFilter("tech");
	if(ret==QString())
		return QDir(getTechPath()).filePath("scmos.tech"); // default technology;
	return QDir(getProcessPath()).filePath(ret);
}

QString Project::getDesignStyleFile()
{
	QString ret = oneLevelFilter("design");
	if(ret==QString())
		return QDir(getTechPath()).filePath("mos.dstyle");
	return QDir(getProcessPath()).filePath(ret);
}

QString Project::getColorMapFile()
{
	QString ret = oneLevelFilter("colors");
	if(ret==QString())
		return QDir(getTechPath()).filePath("mos.cmap");
	return QDir(getProcessPath()).filePath(ret);
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
	if(QFile(getSynthesisScript()).exists()) {
		mainContext.evalFile(getSynthesisScript());
	} else {
		mainContext.evalScript("print \"not defined\"");
	}
}

void Project::simulation()
{
	if(QFile(getSimulationScript()).exists()) {
		mainContext.evalFile(getSimulationScript());
	} else {
		mainContext.evalScript("print \"not defined\"");
	}
	emit(simulationDone());
}

void Project::placement()
{
	if(QFile(getPlacementScript()).exists()) {
		mainContext.evalFile(getPlacementScript());
	} else {
		mainContext.evalScript("print \"not defined\"");
	}
	if(getProjectType().contains("asic"))
		buildPadFrame();
}

void Project::routing()
{
	if(QFile(getRoutingScript()).exists()) {
		mainContext.evalFile(getRoutingScript());
	} else {
		mainContext.evalScript("print \"not defined\"");
	}
}

void Project::buildAll()
{
	synthesis();
	placement();
	routing();
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
	qreal order;
	qreal planes;
	QString plane;

	plane = material2Plane(material);

	if(techDisplayData) {
		order = techDisplayData->getPlaneOrder(plane);
		planes = techDisplayData->getNumPlanes();
		ret = (1/planes)*order;
	}

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

QStringList Project::getRoutingLayers()
{
	QStringList planeList;
	foreach(QString key, lefdata.keys()) {
		foreach(LEFLayerInfo* l, lefdata[key]->getLayers()) {
			if(l->getType()=="ROUTING")
				planeList.append(l->getName());
		}
	}

	return planeList;
}

qreal Project::getRoutingLayerPitch(QString s)
{
	qreal ret = 1000;
	foreach(QString key, lefdata.keys()) {
		foreach(LEFLayerInfo* l, lefdata[key]->getLayers()) {
			if(l->getName()==s)
				ret = l->getPitch();
		}
	}
	return ret;
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

qreal Project::getViaScale(QString s)
{
	foreach(QString key, lefdata.keys()) {
		foreach(lef::LEFVia* v, lefdata[key]->getVias()) {
			if(v) if(v->getName()==s)
				return lefdata[key]->getBaseUnits();
		}
	}
	return 1;
}

lef::LEFVia* Project::getVia(QString s)
{
	foreach(QString key, lefdata.keys()) {
		foreach(lef::LEFVia* v, lefdata[key]->getVias()) {
			if(v) if(v->getName()==s)
				return v;
		}
	}
	return NULL;
}

QStringList Project::getMacroList()
{
	QStringList ret;
	foreach(QString key, lefdata.keys()) {
		foreach(lef::LEFMacro *m, lefdata[key]->getMacros())
			ret << m->getName();
	}
	return ret;
}

QStringList Project::getViaList()
{
	QStringList ret;
	foreach(QString key, lefdata.keys()) {
		foreach(lef::LEFVia *v, lefdata[key]->getVias())
			ret << v->getName();
	}
	return ret;
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

QString Project::getSubBitChar()
{
	QString ret;
	foreach(QString key, lefdata.keys()) {
		ret = lefdata[key]->getSubBitChar();
		if(ret==QString()) continue;
		return ret;
	}
	return "[]";
}

QString Project::getDivideChar()
{
	QString ret;
	foreach(QString key, lefdata.keys()) {
		ret = lefdata[key]->getDivideChar();
		if(ret==QString()) continue;
		return ret;
	}
	return "/";
}

int Project::getSmallestUnit()
{
	int ret = 1;
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

QString Project::getPadFrameScript()
{
	return project_settings->value("padframe_script").toString();
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
	return project_settings->value("routing_script").toString();
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

void Project::setPlacementScript(QString s)
{
	project_settings->setValue("placement_script", s);
	project_settings->sync();
}

void Project::setRoutingScript(QString s)
{
	project_settings->setValue("routing_script", s);
	project_settings->sync();
}

QString Project::getLibertyFile()
{
	QString ret = oneLevelFilter("liberty");
	if(ret==QString())
		return QDir(QDir(getTechPath()).filePath("osu035")).filePath("osu035_stdcells.lib");
	return QDir(getProcessPath()).filePath(ret);
}

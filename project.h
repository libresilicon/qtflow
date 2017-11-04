#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QSettings>
#include <QProcess>
#include <QFileDevice>
#include <QCommandLineParser>
#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QCommandLineParser>
#include <QDomDocument>
#include <QDateTime>
#include <QColor>
#include <QTemporaryDir>
#include <QIcon>
#include <QPixmap>

#include <PythonQt.h>

#include "lef/lefdata.h"
#include "gds2/gdtdata.h"

#include "cmap_reader/colormap.h"
#include "tech_reader/techdata.h"
#include "symbol_reader/symboldata.h"
#include "blif/blifdata.h"

class IProject: public QObject
{
	Q_OBJECT

public:
	IProject();
	virtual ~IProject() {}
	virtual void create(QString) {}
	virtual void simulation() {}
	virtual void synthesis() {}
	virtual void placement() {}
	virtual void routing() {}
	virtual void buildAll() {}
};

class Project : public IProject
{
	Q_OBJECT

public:
	Project(QSettings *settings, QString path, PythonQtObjectPtr main);
	~Project();
	void create(QString);
	void setTechnology(QString tech);
	void setProcess(QString proc);
	void setTopLevel(QString mod);
	void setTestBench(QString mod);
	void setProjectType(QString proc);

	// synthesis flow:
	void synthesis();
	void simulation();
	void placement();
	void routing();
	void buildAll();

	// synthesis flow glue (conversion between file formats):
	void schematics2blif(QString top);
	void blif2cel(QString top);
	void generatecfg(QString top);
	void place2def(QString top);

	bool isMacroCellProject();
	void setSyncLiberty(QString s);
	void setSyncLEF(QString s);
	void setSyncSymbols(QString s);
	QString getSyncLiberty();
	QString getSyncLEF();
	QString getSyncSymbols();

	void createFiles();

	QColor colorMat(QString material);
	QColor colorMaterialTypeMapping(QString material);
	QColor colorFromCode(int i);
	QString layerNameFromCIF(int i);
	QString layerNameFromDStyle(int i);
	QString getLongName(QString m);
	bool hasMaterialTypeMapping(QString material);
	QIcon materialIcon(QString material);
	qreal posMat(QString material);
	qreal thicknessMat(QString material);

	// directories
	QString getSynthesisDir();
	QString getRootDir();
	QString getSourceDir();
	QString getLayoutDir();
	QStringList getSearchDirectories();

	void setSearchDirectories(QStringList l);

	// DRC:
	QStringList getDesignRules();
	TechDesignRule getDesignRule(QString n);

	// special nets
	QStringList getPowerNets();
	QStringList getGroundNets();
	QStringList getClockNets();
	QString getSpecialNetLayer(QString);

	// module names:
	QString getTopLevel();
	QString getTestBench();

	// files for processing:
	QString getTopLevelFile();
	QString getTestBenchFile();
	QString getVCDFile();

	// infos:
	QString getTechnology();
	QString getProcess();
	QString getTechnologyFile();
	QString getTechnologyDisplayFile();
	QString getColorMapFile();
	QString getDesignStyleFile();
	QString getProjectType();
	QString getVCDPath();
	QString getTechPath();
	QString getProcessPath();
	QString getParametersFile();

	QStringList getPlanes();
	QStringList getRoutingLayers();
	qreal getRoutingLayerPitch(QString s);
	QStringList getTypeNames();
	QStringList getTypes(QString s);
	QList<QStringList> getTypesMap(QString s);
	QStringList getAlternativeNames(QString s);

	// python script paths:
	QString getSimulationScript();
	QString getSynthesisScript();
	QString getBLIF2CELScript();
	QString getBLIF2SymbolScript();
	QString getPlace2DEFScript();
	QString getPlacementScript();
	QString getRoutingScript();

	void setSimulationScript(QString s);
	void setSynthesisScript(QString);
	void setPlacementScript(QString s);
	void setRoutingScript(QString s);

	// synthesis options:
	bool hasSettingABC();
	void setSettingABC(bool t);

	// GDS operations:
	bool isDefinedGDSMacro(QString s);
	GDSCell* getGDSMacro(QString s);

	// LEF operations:
	bool isDefinedMacro(QString s);
	lef::LEFMacro* getMacro(QString s);
	QStringList getMacroList();
	int getBaseUnits(QString macro_name);
	int getSmallestUnit();
	QString getSubBitChar();
	QString getDivideChar();

	// Schematics operations:
	bool isDefinedPart(QString s);
	symbol::SchematicsSymbol* getSchematicsPart(QString s);
	QStringList getListOfSchematicParts();
	QStringList getSchematicsLibraryNames();
	QStringList getSchematicsLibraryParts(QString n);

	QString getLibertyFile();

signals:
	void simulationDone();

private:
	QString material2Plane(QString material);

	QStringList twoLevelListFilter(QString filter1, QString filter2);

	QString oneLevelFilter(QString filter);

	// LEF operations:
	QStringList getLibraryFiles();
	void loadLibraryFiles();

	// LEF operations:
	QStringList getGDSFiles();
	void loadGDSFiles();

	// Schematics operations:
	QStringList getSchematicsLibraryFiles();
	void loadSchematicsLibraryFiles();
	void loadScriptFiles();

	QSettings *settings;
	QSettings *project_settings;
	QString rootdir;
	PythonQtObjectPtr mainContext;
	QDomDocument *settingsFileProcess;
	tech::TechData *techDisplayData;
	ColorMap *colorMap;
	QMap<QString,lef::LEFData*> lefdata;
	QMap<QString,symbol::SymbolData*> slibdata;
	QMap<QString,GDTData*> gdtdata;

};

#endif // PROJECT_H

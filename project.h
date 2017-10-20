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

#include "cmap_reader/colormap.h"
#include "tech_reader/techdata.h"
#include "symbol_reader/symboldata.h"

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
	void synthesis();
	void simulation();
	void placement();
	void routing();
	void buildAll();
	void setTechnology(QString tech);
	void setProcess(QString proc);
	void setTopLevel(QString mod);
	void setTestBench(QString mod);
	void setProjectType(QString proc);
	void createFiles();

	QColor colorMat(QString material);
	QColor colorMaterialTypeMapping(QString material);
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
	QStringList getTypeNames();
	QStringList getType(QString s);
	QStringList getAlternativeNames(QString s);

	// python script paths:
	QString getSimulationScript();
	QString getSynthesisScript();
	QString getBLIF2CELScript();
	QString getPlace2DEFScript();
	QString getPlacementScript();
	QString getRoutingScript();

	void setSimulationScript(QString s);
	void setSynthesisScript(QString);
	void setPlacementScript(QString);
	void setRoutingScript(QString);

	// synthesis options:
	bool hasSettingABC();
	void setSettingABC(bool t);

	// LEF operations:
	bool isDefinedMacro(QString s);
	lef::LEFMacro* getMacro(QString s);
	int getBaseUnits(QString macro_name);
	int getSmallestUnit();

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
	// LEF operations:
	QStringList getLibraryFiles();
	void loadLibraryFiles();

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

};

#endif // PROJECT_H

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
	Project(QSettings *settings, QString path, PythonQtObjectPtr *main);
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

	QString getSourceDir();
	QString getTopLevel();
	QString getRootDir();
	QString getTestBench();
	QString getTechnology();
	QString getProcess();
	QString getTechnologyFile();
	QString getTechnologyDisplayFile();
	QString getColorMapFile();
	QString getDesignStyleFile();
	QString getProjectType();
	QString getSynthesisDir();
	QString getLayoutDir();
	QString getVCDFile();
	QString getVCDPath();

	QStringList getPlanes();
	QStringList getTypeNames();
	QStringList getType(QString s);
	QStringList getAlternativeNames(QString s);

	// LEF operations:
	bool isDefinedMacro(QString s);
	lef::LEFMacro* getMacro(QString s);

	// Schematics operations:
	bool isDefinedPart(QString s);
	symbol::SchematicsSymbol* getSchematicsPart(QString s);
	QStringList getListOfSchematicParts();
	QStringList getSchematicsLibraryNames();
	QStringList getSchematicsLibraryParts(QString n);

signals:
	void simulationDone();

private:
	// LEF operations:
	QStringList getLibraryFiles();
	void loadLibraryFiles();

	// Schematics operations:
	QStringList getSchematicsLibraryFiles();
	void loadSchematicsLibraryFiles();

	QSettings *settings;
	QSettings *project_settings;
	QString rootdir;
	PythonQtObjectPtr *mainContext;
	QDomDocument *settingsFileProcess;
	tech::TechData *techDisplayData;
	ColorMap *colorMap;
	QMap<QString,lef::LEFData*> lefdata;
	QMap<QString,symbol::SymbolData*> slibdata;

};

#endif // PROJECT_H

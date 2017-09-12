#ifndef SDEFDATA_H
#define SDEFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace def {
	class DEFModuleInfo {
	public:
		DEFModuleInfo();
		QString module_name;
		QString instance_name;
		double x, y;
	};

	typedef QVector<DEFModuleInfo> mods_t;

	class DEFScanner;
	class DEFData
	{
	public:
		DEFData(QString file);

		// extraction functions
		mods_t getModules();

		class DEFScanner *getLexer();

		// building the info
		void setDistanceUnitMicrons(int t);
		void setAmountComponents(int i);
		void addUsedModuleNames(std::string *module, std::string *name);
		void addUsedModulePlacement(double x, double y);
		void addUsedModule();

	private:
		mods_t parsedModules;
		DEFModuleInfo recent_module;

		class DEFScanner* lexer;
		class DEFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		bool distanceMicrons;
		int distanceMicronsValue;
		int amountComponents;
	};
}

#endif // SDEFDATA_H

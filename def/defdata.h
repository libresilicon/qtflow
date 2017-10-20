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
		QString macro_name;
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
		void addUsedModuleNames(std::string *instance_name, std::string *macro_name);
		void addUsedModulePlacement(double x, double y);
		void addUsedModule();

		// get bbox values
		int getLowerX();
		int getLowerY();
		int getUpperX();
		int getUpperY();

		void setDieArea(int x1, int x2, int y1, int y2);

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

		// bounding rectangle
		int m_BBLowerX;
		int m_BBLowerY;
		int m_BBUpperX;
		int m_BBUpperY;
	};
}

#endif // SDEFDATA_H

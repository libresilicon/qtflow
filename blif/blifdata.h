#ifndef SBLIFDATA_H
#define SBLIFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace blif {
	class BLIFModuleInfo {
	public:
		BLIFModuleInfo();
		QString macro_name;
		QString instance_name;
		double x, y;
	};

	//typeblif QVector<BLIFModuleInfo> mods_t;

	class BLIFScanner;
	class BLIFData
	{
	public:
		BLIFData(QString file);

		class BLIFScanner *getLexer();

		// building the info
		void setDistanceUnitMicrons(int t);
		void setAmountComponents(int i);
		void addUsedModuleNames(std::string *instance_name, std::string *macro_name);
		void addUsedModulePlacement(double x, double y);

		int getDistanceUnit();

		// get bbox values
		int getLowerX();
		int getLowerY();
		int getUpperX();
		int getUpperY();

		void setDieArea(int x1, int x2, int y1, int y2);

	private:
		//mods_t parsedModules;
		BLIFModuleInfo recent_module;

		class BLIFScanner* lexer;
		class BLIFParser* parser;
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

#endif // SBLIFDATA_H

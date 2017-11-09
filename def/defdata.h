#ifndef SDEFDATA_H
#define SDEFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QDebug>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

#include "defmoduleinfo.h"
#include "defrouteinfo.h"
#include "defdatapin.h"

namespace def {
	typedef QVector<DEFModuleInfo> mods_t;
	typedef QVector<DEFDataPin> pins_t;

	class DEFScanner;
	class DEFData
	{
	public:
		DEFData(QString file);

		// extraction functions
		mods_t getModules();
		pins_t getPins();

		class DEFScanner *getLexer();

		// building the info
		void setDistanceUnitMicrons(int t);
		void setAmountComponents(int i);
		void addUsedModuleNames(std::string instance_name, std::string macro_name);
		void addUsedModulePlacement(double x, double y);
		void addUsedModuleOrientation(std::string orient);
		void addUsedModule();

		// pins
		void addUsedPin();
		void addPin(std::string s);
		void setPinLayer(std::string s);
		void setPinPosition(qreal x, qreal y);
		void setPinArea(qreal x1, qreal y1, qreal x2, qreal y2);

		// routes
		void startRoutedInfo(std::string s);
		void startNetConnection(std::string s);
		void addToNetConnection(std::string c, std::string p);

		void setRoutedInfoVia(std::string s);
		void addRoutedInfoPoint(int x, int y);
		void shiftRoutedInfoPointX(int d);
		void shiftRoutedInfoPointY(int d);

		void startNewMetalInfo(std::string s);
		void setNewMetalInfoVia(std::string s);
		void storeNewMetalInfo();
		void addNewMetalPoint(int x, int y);
		void shiftNewMetalPointX(int d);
		void shiftNewMetalPointY(int d);

		int getDistanceUnit();

		// nets and routes
		QStringList getNetList();
		QMap<QString,DEFRouteInfo> getMainRoutes();
		QMap<QString,QVector<DEFRouteInfo> > getSecondaryRoutes();

		// get bbox values
		int getLowerX();
		int getLowerY();
		int getUpperX();
		int getUpperY();

		void setDieArea(qreal x1, qreal y1, qreal x2, qreal y2);

	private:
		mods_t m_parsedModules;
		DEFModuleInfo m_recentModule;

		pins_t m_parsedPins;
		DEFDataPin m_recentPin;

		// nets
		QString m_recentNetName;
		QMap<QString,DEFRouteInfo> m_mainRoutes;
		QMap<QString,QVector<DEFRouteInfo>> m_secondaryRoutes;
		DEFRouteInfo m_recentRoutedInfo;
		QStringList m_netList;

		class DEFScanner* lexer;
		class DEFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		bool distanceMicrons;
		int distanceMicronsValue;
		int amountComponents;

		QPointF m_lastPoint;

		// bounding rectangle
		int m_BBLowerX;
		int m_BBLowerY;
		int m_BBUpperX;
		int m_BBUpperY;
	};
}

#endif // SDEFDATA_H

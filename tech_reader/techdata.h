#ifndef TECHDATA_H
#define TECHDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QRect>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

#include "techdesignrule.h"

namespace tech {
	class TechScanner;
	class TechData
	{
	public:
		TechData(QString file);

		// building the info
		class TechScanner *getLexer();

		void addPlane(std::string);
		void addType(std::string s1, std::string s2);
		void addCIFMapping(int i, std::string layer);
		void addPlaneOrder(int i, std::string plane);

		void addDesignRuleWidth(std::string, int, std::string m);
		void addDesignRuleSpacing(std::string n1, std::string n2, int d, std::string s, std::string m);

		QString layerNameFromCIF(int i);

		QStringList getType(QString s);
		QStringList getDesignRules();
		TechDesignRule getDesignRule(QString n);

		QStringList getPlanes();

		QStringList getTypeNames();

		int getPlaneOrder(QString n);
		int getNumPlanes();

		QStringList getStyleNames();
		QMap<QString,QVector<int>> getStyle(QString s);
		void setRecentStyle(std::string s);
		void addToRecentStyleMember(int i);
		void setRecentStyleMember(std::string s);

	private:
		class TechScanner* lexer;
		class TechParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QStringList planeList;
		QMap<QString,QStringList> typeList;
		QMap<QString,QMap<QString,QVector<int>>> styleList;
		QMap<int,QString> cifLayerNames;
		QMap<QString,int> planeOrder;
		QMap<QString,TechDesignRule> designRules;

		QString recentStyle;
		QString recentStyleMember;

	};
}

#endif // TECHDATA_H

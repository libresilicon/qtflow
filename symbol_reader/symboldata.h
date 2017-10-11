#ifndef SYMBOLDATA_H
#define SYMBOLDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace symbol {
	class SymbolScanner;
	class SymbolData
	{
	public:
		SymbolData(QString file);

		// building the info
		class SymbolScanner *getLexer();

		void addPlane(std::string);
		void addType(std::string s1, std::string s2);

		QStringList getType(QString s);

		QStringList getPlanes();

		QStringList getTypeNames();

		QStringList getStyleNames();
		QMap<QString,QVector<int>> getStyle(QString s);
		void setRecentStyle(std::string s);
		void addToRecentStyleMember(int i);
		void setRecentStyleMember(std::string s);

	private:
		class SymbolScanner* lexer;
		class SymbolParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QStringList planeList;
		QMap<QString,QStringList> typeList;
		QMap<QString,QMap<QString,QVector<int>>> styleList;

		QString recentStyle;
		QString recentStyleMember;

	};
}

#endif // SYMBOLDATA_H

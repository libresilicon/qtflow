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

#include "schematicssymbol.h"

namespace symbol {
	class SymbolScanner;
	class SymbolData
	{
	public:
		SymbolData(QString file);

		// building the info:
		class SymbolScanner *getLexer();

		// getter:
		SchematicsSymbol *getSymbol(QString);
		QStringList getSymbolNames();

		bool isDefinedSymbol(QString name);

		// setter:
		void addPart(std::string name, std::string prefix, int x, int y, std::string a, std::string b, int foo, std::string f, std::string font);
		void addPin(std::string name, int x, int y);
		void addRect(int x1, int y1, int x2, int y2);

	private:
		class SymbolScanner* lexer;
		class SymbolParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QVector<SchematicsSymbol*> partsymbols;
		SchematicsSymbol *recentSymbol;
	};
}

#endif // SYMBOLDATA_H

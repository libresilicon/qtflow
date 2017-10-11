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

		// building the info:
		class SymbolScanner *getLexer();

		// setter:
		void addPart(std::string name, std::string prefix, int x, int y, std::string a, std::string b, std::string f, std::string font);
		void addPin(std::string name, int x, int y);

	private:
		class SymbolScanner* lexer;
		class SymbolParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
	};
}

#endif // SYMBOLDATA_H

#ifndef LEFDATA_H
#define LEFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace lef {
	class LEFScanner;
	class LEFParser;
	class LEFData
	{
	public:
		LEFData();
		LEFData(QString);
		LEFScanner *getLexer();
	private:
		LEFScanner *lexer;
		LEFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
	};
}

#endif // LEFDATA_H

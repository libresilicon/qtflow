#ifndef TECHDATA_H
#define TECHDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace tech {
	class TechScanner;
	class TechData
	{
	public:
		TechData(QString file);

		// building the info
		class TechScanner *getLexer();

	private:
		class TechScanner* lexer;
		class TechParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
	};
}

#endif // TECHDATA_H

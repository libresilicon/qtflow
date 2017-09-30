#ifndef TECHDATA_H
#define TECHDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QStringList>
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

		void addPlane(std::string);
		void addType(std::string s1, std::string s2);

		QStringList getType(QString s);

		QStringList getPlanes();

		QStringList getTypeNames();

	private:
		class TechScanner* lexer;
		class TechParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QStringList planeList;
		QMap<QString,QStringList> typeList;

	};
}

#endif // TECHDATA_H

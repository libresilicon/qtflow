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

		QVector<QString> getType(QString s);

		QStringList getPlanes();

	private:
		class TechScanner* lexer;
		class TechParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QStringList planeList;
		QMap<QString,QVector<QString>> typeList;

	};
}

#endif // TECHDATA_H

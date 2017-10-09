#ifndef SCHEMATICSDATA_H
#define SCHEMATICSDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

#include "schematicswire.h"

namespace schematics {
	class SchematicsScanner;
	class SchematicsData
	{
	public:
		SchematicsData(QString file);
		class SchematicsScanner *getLexer();

		void addWire(std::string name, int x1, int y1, int x2, int y2);

		QVector<SchematicsWire> getWires();

		// get bbox values
		qreal getLowerX();
		qreal getLowerY();
		qreal getUpperX();
		qreal getUpperY();

	private:
		class SchematicsScanner* lexer;
		class SchematicsParser* parser;

		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QVector<SchematicsWire> wires;

		void setBoundaryRectangle(int, int, int, int);

		// bounding rectangle
		int m_BBLowerX;
		int m_BBLowerY;
		int m_BBUpperX;
		int m_BBUpperY;
	};
}

#endif // SCHEMATICSDATA_H

#ifndef MAGICDATA_H
#define MAGICDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>

#include "magicscanner.h"

typedef QVector<QRect> rects_t;
typedef QMap<QString,rects_t> layer_rects_t;

namespace magic {
	class MagicData
	{
	public:
		MagicData();
		MagicData(QString file);

		bool loadFile(QString);

		layer_rects_t getRectangles();
		layer_rects_t getBoxes();

		// building the info
		void addRectangle(int x, int y, int w, int h);
		void addBox(int x, int y, int w, int h);
		void setLayer(std::string *s);
		class MagicScanner *getLexer();

	private:
		layer_rects_t parsedRectangles;
		layer_rects_t parsedBoxes;

		class MagicScanner* lexer;
		class MagicParser* parser;
		QString recentTitle;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
	};
}

#endif // MAGICDATA_H

#ifndef MAGICDATA_H
#define MAGICDATA_H

#include <QString>
#include <QVector>
#include <QRect>

#include "magicscanner.h"

typedef QVector<QRect> rects_t;

namespace magic {
	class MagicData
	{
	public:
		MagicData();
		void clearParsedElements();
		void addRectangle(int x, int y, int w, int h, QString m);
		void setTitle(QString s);
		class MagicScanner *getLexer();
	private:
		rects_t parsedElements;
		class MagicScanner* lexer;
		QString title;
	};
}

#endif // MAGICDATA_H

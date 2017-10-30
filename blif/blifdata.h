#ifndef SBLIFDATA_H
#define SBLIFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

namespace blif {
	class BLIFScanner;
	class BLIFData
	{
	public:
		BLIFData(QString file);

		BLIFScanner* getLexer();

		// build data:
		void setModelName(std::string s);

	private:
		class BLIFScanner* lexer;
		class BLIFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QString m_modelName;

	};
}

#endif // SBLIFDATA_H

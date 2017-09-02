#ifndef VCDATA_H
#define VCDATA_H

#include <QStack>
#include <QHash>

#include <fstream>
#include <istream>

#include "vcdscanner.h"

namespace vcd {
	class VCData
	{
	public:
		VCData(QString s);
		VCDScanner *getLexer();
		void setDate(std::string*);
		void setVersion(std::string*);
		void addTimescale(std::string*);
		void addScope(std::string*);
		vcd_t getVCD();

	private:
		QString streamname;
		QStack<QChar> vcdstring;
		vcd_t parsedVcd;
		VCDScanner *lexer;
		VCDParser *parser;
		bool trace_scanning;
		bool trace_parsing;
		bool rootSet;
	};
}

#endif // VCDATA_H

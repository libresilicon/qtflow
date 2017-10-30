#include "blifdata.h"
#include "blifscanner.h"

#include <QDebug>

namespace blif {
	BLIFData::BLIFData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new BLIFScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new BLIFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	BLIFScanner* BLIFData::getLexer()
	{
		return lexer;
	}

	void BLIFData::setModelName(std::string s)
	{
		m_modelName = QString::fromStdString(s);
	}

}

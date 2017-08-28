#include "lefdata.h"
#include "lefscanner.h"

namespace lef {
	LEFData::LEFData() :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
	}

	LEFData::LEFData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new LEFScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new LEFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}
	
	LEFScanner *LEFData::getLexer()
	{
		return lexer;
	}
}


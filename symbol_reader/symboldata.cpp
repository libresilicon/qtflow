#include "symbol_parser/symbol_parser.h"
#include "symbolscanner.h"
#include "symboldata.h"

namespace symbol {
	SymbolData::SymbolData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new SymbolScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new SymbolParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	SymbolScanner *SymbolData::getLexer()
	{
		return lexer;
	}

	void SymbolData::addPart(std::string name, std::string prefix, int x, int y, std::string a, std::string b, std::string f, std::string font)
	{

	}

	void SymbolData::addPin(std::string name, int x, int y)
	{

	}
}

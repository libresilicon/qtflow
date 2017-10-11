#include "symbol_parser/symbol_parser.h"
#include "symbolscanner.h"

namespace symbol {
	SymbolScanner::SymbolScanner(std::istream* in, std::ostream* out) :
		symbolFlexLexer(in, out)
	{

	}
}

int symbolFlexLexer::yywrap()
{
	return 1;
}

int symbolFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

#include "lefscanner.h"
namespace lef {
	LEFScanner::LEFScanner(std::istream* in, std::ostream* out) :
		lefFlexLexer(in, out)
	{

	}
}

int lefFlexLexer::yywrap()
{
	return 1;
}

int lefFlexLexer::yylex()
{
	std::cerr << "in lefFlexLexer::yylex() !" << std::endl;
	return 0;
}

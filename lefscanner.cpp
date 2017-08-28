#include "lefscanner.h"
namespace lef {
	LEFScanner::LEFScanner()
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

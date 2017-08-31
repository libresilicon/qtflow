#include "defscanner.h"

namespace def {
	DEFScanner::DEFScanner(std::istream* in, std::ostream* out) :
		defFlexLexer(in, out)
	{

	}
}

int defFlexLexer::yywrap()
{
	return 1;
}

int defFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

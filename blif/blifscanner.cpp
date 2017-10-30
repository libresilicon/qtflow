#include "blifscanner.h"

namespace blif {
	BLIFScanner::BLIFScanner(std::istream* in, std::ostream* out) :
		blifFlexLexer(in, out)
	{

	}
}

int blifFlexLexer::yywrap()
{
	return 1;
}

int blifFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

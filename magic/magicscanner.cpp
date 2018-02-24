#include "magic_parser.hpp"
#include "magicscanner.h"

namespace magic {
	MagicScanner::MagicScanner(std::istream* in, std::ostream* out) :
		magicFlexLexer(in, out)
	{

	}
}

int magicFlexLexer::yywrap()
{
	return 1;
}

int magicFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

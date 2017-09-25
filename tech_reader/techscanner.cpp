#include "tech_parser/tech_parser.h"
#include "techscanner.h"

namespace tech {
	TechScanner::TechScanner(std::istream* in, std::ostream* out) :
		techFlexLexer(in, out)
	{

	}
}

int techFlexLexer::yywrap()
{
	return 1;
}

int techFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

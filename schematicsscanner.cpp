#include "schematicsscanner.h"

namespace schematics {
	SchematicsScanner::SchematicsScanner(std::istream* in, std::ostream* out):
		schematicsFlexLexer(in, out)
	{

	}
}

int schematicsFlexLexer::yywrap()
{
	return 1;
}

int schematicsFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

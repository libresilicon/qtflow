#include "vcdscanner.h"

namespace vcd {
	VCDScanner::VCDScanner(std::istream* in, std::ostream* out):
		vcdFlexLexer(in, out)
	{
	}
}

int vcdFlexLexer::yywrap()
{
	return 1;
}

int vcdFlexLexer::yylex()
{
	std::cerr << "in vcdFlexLexer::yylex() !" << std::endl;
	return 0;
}

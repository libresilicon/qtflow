#ifndef VCDSCANNER_H
#define VCDSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer vcdFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "vcd.yy.h"

namespace vcd {
	class VCData;
	class VCDScanner : public vcdFlexLexer
	{
	public:
		VCDScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int vcdlex(YYSTYPE* yylval, vcd::VCData *vcdata);
	};
}

#endif // VCDSCANNER_H

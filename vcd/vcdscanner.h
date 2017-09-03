#ifndef VCDSCANNER_H
#define VCDSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer vcdFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace vcd {
	class VCDParser;
	class VCData;
	class VCDScanner : public vcdFlexLexer
	{
	public:
		VCDScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		vcd::VCDParser::symbol_type vcdlex(vcd::VCDParser::semantic_type* yylval, vcd::VCData *vcdata);
		vcd::VCDParser::symbol_type newlex();
	};
}

#endif // VCDSCANNER_H

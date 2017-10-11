#ifndef SYMBOLSCANNER_H
#define SYMBOLSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer symbolFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace symbol {
	class SymbolScanner : public symbolFlexLexer
	{
	public:
		SymbolScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int symbollex(symbol::SymbolParser::semantic_type*, SymbolData*);
	};
}

#endif // SYMBOLSCANNER_H

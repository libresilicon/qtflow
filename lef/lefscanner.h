#ifndef LEFSCANNER_H
#define LEFSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer lefFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "lef_parser.hpp"

namespace lef {
	class LEFData;
	class LEFScanner : public lefFlexLexer
	{
	public:
		LEFScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int leflex(lef::LEFParser::semantic_type*, lef::LEFData*);
	};
}

#endif // LEFSCANNER_H

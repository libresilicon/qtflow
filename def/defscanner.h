#ifndef DEFSCANNER_H
#define DEFSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer defFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "def.yy.h"

namespace def {
	class DEFScanner : public defFlexLexer
	{
	public:
		DEFScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int deflex(DEFParser::semantic_type*, DEFData*);
	};
}

#endif // DEFSCANNER_H

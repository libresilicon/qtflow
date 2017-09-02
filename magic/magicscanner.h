#ifndef MAGICSCANNER_H
#define MAGICSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer magicFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace magic {
	class MagicScanner : public magicFlexLexer
	{
	public:
		MagicScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int magiclex(magic::MagicParser::semantic_type*, MagicData*);
	};
}

#endif // MAGICSCANNER_H

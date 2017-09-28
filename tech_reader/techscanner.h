#ifndef TECHSCANNER_H
#define TECHSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer techFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace tech {
	class TechScanner : public techFlexLexer
	{
	public:
		TechScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int techlex(tech::TechParser::semantic_type*, TechData*);
	};
}

#endif // TECHSCANNER_H

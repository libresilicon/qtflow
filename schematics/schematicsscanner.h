#ifndef SCHEMATICSSCANNER_H
#define SCHEMATICSSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer schematicsFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "schematics_parser/schematics_parser.h"

namespace schematics {
	class SchematicsScanner : public schematicsFlexLexer
	{
	public:
		SchematicsScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int schematicslex(schematics::SchematicsParser::semantic_type*,schematics::SchematicsData*);
	};
}

#endif // SCHEMATICSSCANNER_H

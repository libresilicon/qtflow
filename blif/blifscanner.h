#ifndef BLIFSCANNER_H
#define BLIFSCANNER_H

#ifndef __FLEX_LEXER_H
#define yyFlexLexer blifFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include "blif_parser/blif_parser.h"

namespace blif {
	class BLIFScanner : public blifFlexLexer
	{
	public:
		BLIFScanner(std::istream* in = 0, std::ostream* out = &std::cout);
		int bliflex(BLIFParser::semantic_type*, BLIFData*);
	};
}

#endif // BLIFSCANNER_H

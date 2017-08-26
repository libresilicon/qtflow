%{
#include <cstdlib>
#include <QString>

#include "magicscanner.h"

#define YY_DECL int magic::MagicScanner::magiclex(magic::MagicParser::semantic_type* magiclval, magic::MagicData *magicdata)
%}

%option header-file="magic.ll.h"
%option prefix="magic"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput

%%

#.*         { /* comment */ }

magic							{ return magic::MagicParser::token::MAGIC; }
tech							{ return magic::MagicParser::token::TECH; }
magscale						{ return magic::MagicParser::token::MAGSCALE; }
timestamp						{ return magic::MagicParser::token::TIMESTAMP; }
rect							{ return magic::MagicParser::token::RECT; }
rlabel							{ return magic::MagicParser::token::RLABEL; }
\<\<\							{ return magic::MagicParser::token::BEGINTITLE; }
\ \>\>							{ return magic::MagicParser::token::ENDTITLE; }
\<\<\ end\ \>\>					{ return magic::MagicParser::token::END; }
[[:alpha:]][[:alnum:]<>]+		{ magiclval->v_str = yytext; return magic::MagicParser::token::IDENT; }
[0-9-]+							{ magiclval->v_int = atoi(yytext); return magic::MagicParser::token::INTEGER; }
[ \t]							{ return magic::MagicParser::token::SPACE; }
\r\n							{/* skip */}
\n								{/* skip */}

%%

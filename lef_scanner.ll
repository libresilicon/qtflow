%{
#include <cstdlib>
#include <QString>

#include "lefscanner.h"

#define YY_DECL int lef::LEFScanner::leflex(\
	lef::LEFParser::semantic_type* leflval,\
	lef::LEFData *lefdata)
%}

%option header-file="lef.ll.h"
%option prefix="lef"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

VERSION		"VERSION"

%%

{VERSION}+		{ return lef::LEFParser::token::VERSION; }

-[0-9]+|[0-9]+ {
	leflval->v_int = atoi(yytext);
	return lef::LEFParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	leflval->v_double = atof(yytext);
	return lef::LEFParser::token::DOUBLE;
}

[A-Za-z][A-Za-z0-9_,.-<>]* {
	leflval->v_str = new std::string(yytext, yyleng);
	return lef::LEFParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}


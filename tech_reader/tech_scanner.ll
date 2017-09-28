%{
#include <cstdlib>
#include <QString>

#include "tech_parser/tech_parser.h"
#include "tech_reader/techscanner.h"

#define YY_DECL int tech::TechScanner::techlex(\
	tech::TechParser::semantic_type* techlval,\
	tech::TechData *techdata)
%}

%option prefix="tech"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

TECH		"tech"
END			"end"
FORMAT		"format"

MAGSCALE	"magscale"
TIMESTAMP	"timestamp"
RECT		"rect"
USE			"use"
RLABEL		"rlabel"
FLABEL		"flabel"
BEGINTITLE	"<<"
ENDTITLE	">>"
BOX			"box"
PORT		"port"
TRANSFORM	"transform"

%%

{TECH}+				{ return tech::TechParser::token::TECH; }
{FORMAT}+			{ return tech::TechParser::token::FORMAT; }
{END}+				{ return tech::TechParser::token::END; }

-[0-9]+|[0-9]+ {
	techlval->v_int = atoi(yytext);
	return tech::TechParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	techlval->v_double = atof(yytext);
	return tech::TechParser::token::DOUBLE;
}

[A-Za-z][A-Za-z0-9_,.-<>]* {
	techlval->v_str = new std::string(yytext, yyleng);
	return tech::TechParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}

%%

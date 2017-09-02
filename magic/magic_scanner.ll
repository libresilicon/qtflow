%{
#include <cstdlib>
#include <QString>

#include "magic_parser/magic_parser.h"
#include "magic/magicscanner.h"

#define YY_DECL int magic::MagicScanner::magiclex(\
	magic::MagicParser::semantic_type* magiclval,\
	magic::MagicData *magicdata)
%}

%option prefix="magic"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

MAGIC		"magic"
TECH		"tech"
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

{TRANSFORM}+		{ return magic::MagicParser::token::TRANSFORM; }
{PORT}+				{ return magic::MagicParser::token::PORT; }
{BOX}+				{ return magic::MagicParser::token::BOX; }
{USE}+				{ return magic::MagicParser::token::USE; }
{MAGIC}+			{ return magic::MagicParser::token::MAGIC; }
{TECH}+				{ return magic::MagicParser::token::TECH; }
{MAGSCALE}+			{ return magic::MagicParser::token::MAGSCALE; }
{TIMESTAMP}+		{ return magic::MagicParser::token::TIMESTAMP; }
{RECT}+				{ return magic::MagicParser::token::RECT; }
{RLABEL}+			{ return magic::MagicParser::token::RLABEL; }
{FLABEL}+			{ return magic::MagicParser::token::FLABEL; }
{BEGINTITLE}+		{ return magic::MagicParser::token::BEGINTITLE; }
{ENDTITLE}+			{ return magic::MagicParser::token::ENDTITLE; }

-[0-9]+|[0-9]+ {
	magiclval->v_int = atoi(yytext);
	return magic::MagicParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	magiclval->v_double = atof(yytext);
	return magic::MagicParser::token::DOUBLE;
}

[A-Za-z][A-Za-z0-9_,.-<>]* {
	magiclval->v_str = new std::string(yytext, yyleng);
	return magic::MagicParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}

%%

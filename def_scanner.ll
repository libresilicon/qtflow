%{
#include <cstdlib>
#include <QString>

#include "defscanner.h"

#define YY_DECL int def::DEFScanner::deflex(\
	def::DEFParser::semantic_type* deflval,\
	def::DEFData *defdata)
%}

%option header-file="def.ll.h"
%option prefix="def"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

MAGIC		"def"
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

{TRANSFORM}+		{ return def::DEFParser::token::TRANSFORM; }
{PORT}+				{ return def::DEFParser::token::PORT; }
{BOX}+				{ return def::DEFParser::token::BOX; }
{USE}+				{ return def::DEFParser::token::USE; }
{MAGIC}+			{ return def::DEFParser::token::MAGIC; }
{TECH}+				{ return def::DEFParser::token::TECH; }
{MAGSCALE}+			{ return def::DEFParser::token::MAGSCALE; }
{TIMESTAMP}+		{ return def::DEFParser::token::TIMESTAMP; }
{RECT}+				{ return def::DEFParser::token::RECT; }
{RLABEL}+			{ return def::DEFParser::token::RLABEL; }
{FLABEL}+			{ return def::DEFParser::token::FLABEL; }
{BEGINTITLE}+		{ return def::DEFParser::token::BEGINTITLE; }
{ENDTITLE}+			{ return def::DEFParser::token::ENDTITLE; }

-[0-9]+|[0-9]+ {
	deflval->v_int = atoi(yytext);
	return def::DEFParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	deflval->v_double = atof(yytext);
	return def::DEFParser::token::DOUBLE;
}

[A-Za-z][A-Za-z0-9_,.-<>]* {
	deflval->v_str = new std::string(yytext, yyleng);
	return def::DEFParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}

%%

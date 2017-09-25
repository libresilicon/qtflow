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

MAGIC		"tech"
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

{TRANSFORM}+		{ return tech::TechParser::token::TRANSFORM; }
{PORT}+				{ return tech::TechParser::token::PORT; }
{BOX}+				{ return tech::TechParser::token::BOX; }
{USE}+				{ return tech::TechParser::token::USE; }
{MAGIC}+			{ return tech::TechParser::token::MAGIC; }
{TECH}+				{ return tech::TechParser::token::TECH; }
{MAGSCALE}+			{ return tech::TechParser::token::MAGSCALE; }
{TIMESTAMP}+		{ return tech::TechParser::token::TIMESTAMP; }
{RECT}+				{ return tech::TechParser::token::RECT; }
{RLABEL}+			{ return tech::TechParser::token::RLABEL; }
{FLABEL}+			{ return tech::TechParser::token::FLABEL; }
{BEGINTITLE}+		{ return tech::TechParser::token::BEGINTITLE; }
{ENDTITLE}+			{ return tech::TechParser::token::ENDTITLE; }

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

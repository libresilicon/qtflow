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

NUMBER			[0-9]
INTEGER			-{NUMBER}+|{NUMBER}+
EXPONENT		[eE][+-]?{INTEGER}
DOUBLE			{INTEGER}("."{INTEGER})?{EXPONENT}?

STRING			[A-Za-z]|[A-Za-z0-9_,.-<>'"'\[\]\/]

SEMICOLON			";"
COMMENT				"#"
VERSION				"VERSION"
NAMESCASESENSITIVE	"NAMESCASESENSITIVE"

%%
{SEMICOLON}.*				{}
{COMMENT}.*					{}
{VERSION}+					{ return def::DEFParser::token::VERSION; }
{NAMESCASESENSITIVE}+		{ return def::DEFParser::token::NAMESCASESENSITIVE; }

{INTEGER}* {
	deflval->v_int = atoi(yytext);
	return def::DEFParser::token::INTEGER;
}

{DOUBLE}* {
	deflval->v_double = atof(yytext);
	return def::DEFParser::token::DOUBLE;
}

{STRING}* {
	deflval->v_str = new std::string(yytext, yyleng);
	return def::DEFParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}

%%

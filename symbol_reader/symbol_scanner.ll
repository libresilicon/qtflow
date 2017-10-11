%{
#include <cstdlib>
#include <QString>

#include "symbol_parser/symbol_parser.h"
#include "symbol_reader/symbolscanner.h"

#define YY_DECL int symbol::SymbolScanner::symbollex(\
	symbol::SymbolParser::semantic_type* symbollval,\
	symbol::SymbolData *symboldata)
%}

%option prefix="symbol"
%option c++
%option batch
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

STRING				[A-Za-z]|[~A-Za-z0-9_,.\-<>\[\]\/\(\)$\*\'=]
HASH				"#"
QUOTE				"\""

DEF					"DEF"
ENDDEF				"ENDDEF"

DEF_X				"X"
DEF_DRAW			"DRAW"
DEF_ENDDRAW			"ENDDRAW"

%x component

%%

{DEF}+								{
	BEGIN(component);
	return symbol::SymbolParser::token::DEF;
}

<component>{DEF_X}+					{
	return symbol::SymbolParser::token::X;
}

<component>{DEF_DRAW}+				{
	return symbol::SymbolParser::token::DRAW;
}

<component>{DEF_ENDDRAW}+			{
	return symbol::SymbolParser::token::ENDDRAW;
}

<component>{ENDDEF}+				{
	BEGIN(INITIAL);
	return symbol::SymbolParser::token::ENDDEF;
}

<component>-[0-9]+|[0-9]+			{
	symbollval->v_int = atoi(yytext);
	return symbol::SymbolParser::token::INTEGER;
}

<component>[0-9]+"."[0-9]*			{
	symbollval->v_double = atof(yytext);
	return symbol::SymbolParser::token::DOUBLE;
}

<component>{STRING}*				{
	symbollval->v_str = new std::string(yytext, yyleng);
	return symbol::SymbolParser::token::STRING;
}

<component>[ \n\t\r]+				{}

-[0-9]+|[0-9]+						{
	symbollval->v_int = atoi(yytext);
	return symbol::SymbolParser::token::INTEGER;
}

[0-9]+"."[0-9]*						{
	symbollval->v_double = atof(yytext);
	return symbol::SymbolParser::token::DOUBLE;
}

{STRING}*							{
	symbollval->v_str = new std::string(yytext, yyleng);
	return symbol::SymbolParser::token::STRING;
}

[ \n\t\r]+							{}


%%

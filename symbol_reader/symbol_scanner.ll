%{
#include <cstdlib>
#include <QString>

#include "symbol_parser.hpp"
#include "symbolscanner.h"

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

STRING				[A-Za-z]|[~A-Za-z0-9_,.\-<>\[\]\/\(\)$\*\'=\"]
HASH				"#"
QUOTE				"\""

BEGINLIB			"EESchema-LIBRARY"

DEF					"DEF"
ENDDEF				"ENDDEF"

DEF_X				"X"
DEF_F				"F"
DEF_C				"C"
DEF_S				"S"

DEF_DRAW			"DRAW"
DEF_ENDDRAW			"ENDDRAW"

%x pin
%x field

%%

{HASH}.*							{}

{BEGINLIB}.*						{
	return symbol::SymbolParser::token::BEGINLIB;
}

{DEF}+								{
	return symbol::SymbolParser::token::DEF;
}

{ENDDEF}+							{
	return symbol::SymbolParser::token::ENDDEF;
}

{DEF_F}[0-9]+						{
	BEGIN(field);
	return symbol::SymbolParser::token::F;
}

{DEF_C}+							{
	return symbol::SymbolParser::token::C;
}

{DEF_X}+							{
	BEGIN(pin);
	return symbol::SymbolParser::token::X;
}

{DEF_S}+							{
	return symbol::SymbolParser::token::S;
}

{DEF_DRAW}+							{
	return symbol::SymbolParser::token::DRAW;
}

{DEF_ENDDRAW}+						{
	return symbol::SymbolParser::token::ENDDRAW;
}

<field>[ \t\r]+				{}
<field>[\r\n]+				{
	BEGIN(INITIAL);
}

<field>-[0-9]+|[0-9]+		{
	symbollval->v_int = atoi(yytext);
	return symbol::SymbolParser::token::INTEGER;
}

<field>[0-9]+"."[0-9]*		{
	symbollval->v_double = atof(yytext);
	return symbol::SymbolParser::token::DOUBLE;
}

<field>{STRING}*			{
	symbollval->v_str = new std::string(yytext, yyleng);
	return symbol::SymbolParser::token::STRING;
}

<pin>[ \t\r]+				{}
<pin>[\r\n]+				{
	BEGIN(INITIAL);
}

<pin>-[0-9]+|[0-9]+			{
	symbollval->v_int = atoi(yytext);
	return symbol::SymbolParser::token::INTEGER;
}

<pin>[0-9]+"."[0-9]*			{
	symbollval->v_double = atof(yytext);
	return symbol::SymbolParser::token::DOUBLE;
}

<pin>{STRING}*				{
	symbollval->v_str = new std::string(yytext, yyleng);
	return symbol::SymbolParser::token::STRING;
}

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

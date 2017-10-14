%define api.prefix {symbol}
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {SymbolParser}

%code requires {
	namespace symbol {
		class SymbolData;
	}
}

%param {symbol::SymbolData *symboldata}

%{
#include <iostream>
#include <string>
#include <QString>

#include "symbol_parser/symbol_parser.h"
#include "symbol_reader/symboldata.h"
#include "symbol_reader/symbolscanner.h"

#define symbollex (symboldata->getLexer())->symbollex
#define symbollineno (int)(symboldata->getLexer())->lineno()
#define yytext (symboldata->getLexer())->YYText()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token BEGINLIB

%token DEF
%token X
%token F
%token C
%token S
%token ENDDEF

%token DRAW
%token ENDDRAW

%token <v_int>		INTEGER
%token <v_str>		STRING
%token <v_double>	DOUBLE

%start symbol_file

%%

symbol_file: BEGINLIB symbol_sections;

symbol_sections:
	  symbol_section
	| symbol_sections symbol_section
;

symbol_section:
	  symbol_header fields DRAW drawable_list ENDDRAW ENDDEF
;

symbol_header:
DEF STRING STRING INTEGER INTEGER STRING STRING INTEGER STRING STRING
{
	symboldata->addPart(*$2,*$3,$4,$5,*$6,*$7,$8,*$9,*$10);
}
;

fields:
	  field
	| fields field
;

field:
	F STRING INTEGER INTEGER INTEGER STRING STRING STRING STRING
;

drawable_list:
	  drawable_entry
	| drawable_list drawable_entry
;

drawable_entry:
	  pin
	| crect
	| srect
;

crect: C INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER STRING;
srect:
S INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER STRING
{
	symboldata->addRect($2,$3,$4,$5);
};

pin:
X STRING INTEGER INTEGER INTEGER INTEGER STRING INTEGER INTEGER INTEGER INTEGER STRING
{
	symboldata->addPin(*$2,$3,$4,$5,$6,*$7,$8,$9,$10,$11,*$12);
}
;

%%

void symbol::SymbolParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << symbollineno << ", yytext: " << yytext <<std::endl;
}

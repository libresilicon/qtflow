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

%token DEF
%token X
%token ENDDEF

%token DRAW
%token ENDDRAW

%token <v_int>		INTEGER
%token <v_str>		STRING
%token <v_double>	DOUBLE

%start symbol_file

%%

symbol_file:
	symbol_sections
;

symbol_sections:
	  symbol_section
	| symbol_sections symbol_section
;

symbol_section: DEF STRING STRING INTEGER INTEGER STRING STRING INTEGER STRING STRING pins ENDDEF;

pins:
	| DRAW pin_list ENDDRAW;

pin_list:
	  pin_entry
	| pin_list pin_entry
;

pin_entry:
	X STRING INTEGER INTEGER INTEGER INTEGER STRING INTEGER INTEGER INTEGER INTEGER STRING STRING
;

%%

void symbol::SymbolParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << symbollineno << ", yytext: " << yytext <<std::endl;
}

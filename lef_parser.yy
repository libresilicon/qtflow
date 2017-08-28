%define api.prefix {lef}
%defines "lef.yy.h"
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {LEFParser}

%code requires {
	namespace lef {
		class LEFData;
	}
}

%param {class lef::LEFData *lefdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "lefdata.h"
#include "lefscanner.h"

#define leflex (lefdata->getLexer())->leflex
#define leflineno (int)(lefdata->getLexer())->lineno()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%token VERSION
%token NAMESCASESENSITIVE
%token BUSBITCHARS

%%

version: VERSION DOUBLE

%%

void lef::LEFParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << leflineno << std::endl;
}

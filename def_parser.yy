%define api.prefix {def}
%defines "def.yy.h"
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {DEFParser}

%code requires {
	namespace def {
		class DEFData;
	}
}

%param {def::DEFData *defdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "defscanner.h"
#include "defdata.h"

#define deflex (defdata->getLexer())->deflex
#define deflineno (int)(defdata->getLexer())->lineno()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token VERSION
%token NAMESCASESENSITIVE

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%%

version: VERSION DOUBLE;

%%

void def::DEFParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << deflineno << std::endl;
}

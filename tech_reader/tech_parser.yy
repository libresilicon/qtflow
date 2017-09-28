%define api.prefix {tech}
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {TechParser}

%code requires {
	namespace tech {
		class TechData;
	}
}

%param {tech::TechData *techdata}

%{
#include <iostream>
#include <string>
#include <QString>

#include "tech_parser/tech_parser.h"
#include "tech_reader/techdata.h"
#include "tech_reader/techscanner.h"

#define techlex (techdata->getLexer())->techlex
#define techlineno (int)(techdata->getLexer())->lineno()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token TECH
%token FORMAT
%token END

%token VERSION
%token DESRIPTION

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%start tech_file
%%
tech_file:
	  tech_section
	| tech_file tech_section
;

tech_section:
	  TECH tech_header END
	| VERSION tech_version END
;

tech_header_section:
	  FORMAT INTEGER
	| STRING
;

tech_header:
	  tech_header_section
	| tech_header tech_header_section
;

tech_version_section:
	  VERSION STRING
	| DESRIPTION STRING
;

tech_version:
	  tech_version_section
	| tech_version tech_version_section
;

%%

void tech::TechParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << techlineno << std::endl;
}

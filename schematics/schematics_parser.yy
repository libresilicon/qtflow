%error-verbose
%define "parser_class_name" {SchematicsParser}
%define api.prefix {schematics}
%language "c++"

%code requires {
	namespace schematics {
		class SchematicsData;
	}
}
%param {schematics::SchematicsData *schematicsdata}

%{
#include <iostream>
#include <string>

#include <QString>
#include <QMap>
#include <QStack>

#include "schematics/schematicsscanner.h"
#include "schematics/schematicsdata.h"

#define schematicslex (schematicsdata->getLexer())->schematicslex
#define schematicslineno (int)(schematicsdata->getLexer())->lineno()
#define schematicstext (schematicsdata->getLexer())->YYText()

%}

%union {
	std::string* v_str;
	double v_double;
	int v_int;
}

%token COMPONENT
%token END_COMPONENT

%token DESCR
%token END_DESCR

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%start schematics_file

%%

schematics_file:
	| schematics_entry
	| schematics_file schematics_entry
;

schematics_entry:
	| description
	| component
;

description:
	DESCR STRING INTEGER INTEGER description_list END_DESCR
;

description_list:
	  description_content
	| description_list description_content
;

description_content:
	| STRING
	| INTEGER
;

component:
	COMPONENT component_list END_COMPONENT
;

component_list:
	  component_content
	| component_list component_content
;

component_content:
	  | STRING
	  | INTEGER
;

%%

void schematics::SchematicsParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << schematicslineno << " yytext: " << schematicstext << std::endl;
}

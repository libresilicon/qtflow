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

%token PLANES
%token TYPES
%token CONTACT
%token STACKABLE

%token STYLES
%token STYLETYPE
%token STYLE

%token COMPOSE
%token PAINT
%token ERASE

%token CONNECT
%token CIFOUTPUT
%token CIFINPUT
%token SCALEFACTOR
%token LAYER
%token BLOAT_OR
%token ASTERISK
%token SHRINK
%token GROW
%token LABELS
%token CALMA
%token TEMPLAYER
%token AND_NOT
%token AND
%token OR
%token SQUARES
%token IGNORE

%token <v_int>		INTEGER
%token <v_str>		STRING
%token <v_double>	DOUBLE

%token <std::string> Multiline "multiline-string"

%start tech_file
%%
tech_file:
	  tech_section
	| tech_file tech_section
;

tech_section:
	  TECH tech_header END
	| VERSION tech_version END
	| PLANES plane_list END
	| TYPES type_list END
	| CONTACT contact_list END
	| STYLES style_list END
	| COMPOSE compose_list END
	| CONNECT connect_list END
	| CIFOUTPUT cifoutput_list END
	| CIFINPUT cifinput_list END
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
	  Multiline
	| DESRIPTION Multiline
;

tech_version:
	  tech_version_section
	| tech_version tech_version_section
;

plane_list:
	  plane_entry
	| plane_entry plane_list
;

plane_entry: STRING | CONTACT;

type_list:
	  type_entry
	| type_entry type_list
;

type_entry: STRING STRING | CONTACT STRING;

contact_list:
	  contact_entry
	| contact_entry contact_list
;

contact_entry: STRING STRING STRING | STACKABLE STRING STRING STRING;

style_list:
	  style_entry
	| style_entry style_list
;

style_entry:
	  STYLETYPE STRING
	| STRING INTEGER
;

compose_list:
	  compose_entry
	| compose_entry compose_list
;

compose_entry:
	  COMPOSE STRING STRING STRING
	| PAINT STRING STRING STRING
	| ERASE STRING STRING STRING
;

connect_list:
	  connect_entry
	| connect_entry connect_list
;

connect_entry: STRING STRING;

cifoutput_list:
	  cifoutput_entry
	| cifoutput_entry cifoutput_list
;

cifoutput_entry:
	  STYLE Multiline
	| SCALEFACTOR INTEGER INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING ASTERISK INTEGER
	| BLOAT_OR STRING ASTERISK INTEGER STRING INTEGER
	| SHRINK INTEGER
	| GROW INTEGER
	| LABELS STRING
	| CALMA INTEGER INTEGER
	| TEMPLAYER STRING
	| TEMPLAYER STRING STRING
	| AND_NOT STRING
	| OR STRING
	| AND STRING
	| SQUARES INTEGER INTEGER INTEGER
;

cifinput_list:
	  cifinput_entry
	| cifinput_entry cifinput_list
;

cifinput_entry:
	  STYLE Multiline
	| SCALEFACTOR INTEGER
	| SCALEFACTOR INTEGER INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING INTEGER
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING ASTERISK INTEGER
	| BLOAT_OR STRING ASTERISK INTEGER STRING INTEGER
	| SHRINK INTEGER
	| GROW INTEGER
	| LABELS STRING
	| CALMA INTEGER INTEGER
	| CALMA STRING INTEGER ASTERISK
	| CALMA INTEGER INTEGER ASTERISK
	| TEMPLAYER STRING
	| TEMPLAYER STRING STRING
	| AND_NOT STRING
	| OR STRING
	| AND STRING
	| SQUARES INTEGER INTEGER INTEGER
	| IGNORE STRING
;

%%

void tech::TechParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << techlineno << std::endl;
}

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

%token PORT
%token BOX
%token TRANSFORM
%token MAGIC
%token TECH
%token MAGSCALE
%token TIMESTAMP
%token BEGINTITLE
%token ENDTITLE
%token USE

%token RECT
%token RLABEL
%token FLABEL

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%start tech_file
%%
tech_file: MAGIC technology magscale timestamp sections;
technology: TECH STRING
	{
		techdata->setTechnology($2);
	}
	;
magscale: MAGSCALE INTEGER INTEGER;
timestamp: TIMESTAMP INTEGER;
sections:
	  sections section
	| section
	;

section:
	  BEGINTITLE sectiontitle ENDTITLE items;
	| BEGINTITLE sectiontitle ENDTITLE;

sectiontitle: STRING
	{
		techdata->setLayer($1);
	}
	;

items: items item | item ;

item:
	  rect
	| rlabel
	| flabel
	| port
	| use
	;

use: use_start timestamp transform use_box
	{
		techdata->addUsedModule();
	}
	;

use_start: USE STRING STRING
	{
		techdata->addUsedModuleNames($2,$3);
	}
	;

transform:
	TRANSFORM INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER
	{
		techdata->addUsedModuleTransform($2,$3,$4,$5,$6,$7);
	}
	;

use_box:
	BOX INTEGER INTEGER INTEGER INTEGER
	{
		techdata->addUsedModuleBox($2, $3, $4, $5);
	}
	;

rect:
	RECT INTEGER INTEGER INTEGER INTEGER
	{
		techdata->addRectangle($2, $3, $4, $5);
	}
    ;

rlabel:
	RLABEL STRING
	INTEGER INTEGER INTEGER INTEGER INTEGER
	STRING
	;

flabel:
	FLABEL STRING
	INTEGER INTEGER INTEGER INTEGER INTEGER
	STRING
	INTEGER INTEGER INTEGER INTEGER
	STRING
	;

port: PORT INTEGER STRING;

%%

void tech::TechParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << techlineno << std::endl;
}

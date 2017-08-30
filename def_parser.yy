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

%param {class def::DEFData *defdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "defdata.h"
#include "defscanner.h"

#define deflex (defdata->getLexer())->deflex
#define deflineno (int)(defdata->getLexer())->lineno()

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

%token INTEGER
%type <v_int> INTEGER
%token STRING
%type <v_str> STRING
%token DOUBLE
%type <v_double> DOUBLE

%start def_file
%%
def_file: MAGIC technology magscale timestamp sections;
technology: TECH STRING
	{
		defdata->setTechnology($2);
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
		defdata->setLayer($1);
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
		defdata->addUsedModule();
	}
	;

use_start: USE STRING STRING
	{
		defdata->addUsedModuleNames($2,$3);
	}
	;

transform:
	TRANSFORM INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER
	{
		defdata->addUsedModuleTransform($2,$3,$4,$5,$6,$7);
	}
	;

use_box:
	BOX INTEGER INTEGER INTEGER INTEGER
	{
		defdata->addUsedModuleBox($2, $3, $4, $5);
	}
	;

rect:
	RECT INTEGER INTEGER INTEGER INTEGER
	{
		defdata->addRectangle($2, $3, $4, $5);
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

void def::DEFParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << deflineno << std::endl;
}

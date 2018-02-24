%define api.prefix {magic}
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {MagicParser}

%code requires {
	namespace magic {
		class MagicData;
	}
}

%param {magic::MagicData *magicdata}

%{
#include <iostream>
#include <string>
#include <QString>

#include "magic_parser.hpp"
#include "magic/magicdata.h"
#include "magic/magicscanner.h"

#define magiclex (magicdata->getLexer())->magiclex
#define magiclineno (int)(magicdata->getLexer())->lineno()

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

%start magic_file
%%
magic_file: MAGIC magic_entries;

magic_entries:
	  magic_entry
	| magic_entries magic_entry;

magic_entry:
	  technology
	| magscale
	| timestamp
	| section
	| use
;

technology: TECH STRING
	{
		magicdata->setTechnology($2);
	}
	;

magscale: MAGSCALE INTEGER INTEGER;
timestamp: TIMESTAMP INTEGER;

section:
	  BEGINTITLE sectiontitle ENDTITLE items;
	| BEGINTITLE sectiontitle ENDTITLE;

sectiontitle: STRING
	{
		magicdata->setLayer($1);
	}
	;

items:
	  item
	| items item;

item:
	  rect
	| rlabel
	| flabel
	| port
	;

use:
use_start timestamp transform use_box
{
	magicdata->addUsedModule();
};

use_start:
USE STRING STRING
{
	magicdata->addUsedModuleNames($2,$3);
};

transform:
	TRANSFORM INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER
	{
		magicdata->addUsedModuleTransform($2,$3,$4,$5,$6,$7);
	}
	;

use_box:
	BOX INTEGER INTEGER INTEGER INTEGER
	{
		magicdata->addUsedModuleBox($2, $3, $4, $5);
	}
	;

rect:
	RECT INTEGER INTEGER INTEGER INTEGER
	{
		magicdata->addRectangle($2, $3, $4, $5);
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

void magic::MagicParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << magiclineno << std::endl;
}

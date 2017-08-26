%define api.prefix {magic}
%defines "magic.yy.h"
%error-verbose
%language "c++"
%glr-parser
%token-table
%define "parser_class_name" {MagicParser}

%code requires {
	namespace magic {
		class MagicData;
	}
}

%param {class magic::MagicData *magicdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "magicdata.h"

#define magiclex (magicdata->getLexer())->magiclex
//#define yylineno (int)magicdata->lexer->lineno()
//#define yytext magicdata->lexer->YYText()

%}

%union {
	int v_int;
	char* v_str;
}

%initial-action
{
	magicdata->clearParsedElements();
}

%token SPACE
%token MAGIC TECH MAGSCALE
%token TIMESTAMP
%token BEGINTITLE ENDTITLE

%token RECT
%token RLABEL

%token END

%token INTEGER
%type <v_int> INTEGER
%token IDENT
%type <v_str> IDENT

%%

program:
	MAGIC
	TECH SPACE IDENT
	MAGSCALE SPACE INTEGER SPACE INTEGER
	TIMESTAMP SPACE INTEGER
    sections
    ;

sections:
	sections section
    |
    section
    ;

section:
	BEGINTITLE sectiontitle ENDTITLE items
    |
    END
    |
    %empty
    ;

sectiontitle:
	IDENT { magicdata->setTitle(QString($1)); }
    ;

items:
	items item
    |
    item
    ;

item:
    rect
    |
    rlabel
    |
    sections
    ;

rect:
	RECT SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER { magicdata->addRectangle($3, $5, $7 - $3, $9 - $5, "layer"); }
    ;

rlabel:
    RLABEL SPACE material
    SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE
    IDENT
    ;

material:
    IDENT
    ;
%%

void magic::MagicParser::error(const std::string &s) {
	//yyclearin;
	//throw ParserException{magicline, QString(s)};
}

%{
#include "common.h"

#include <iostream>
#include <string>

#include <QString>

int yylex();
void yyerror(const char *s);

rects_t parsedElements;
int magicline;
QString title;

%}

%defines "yymagic.h"
%output "yymagic.cpp"
%error-verbose

%define api.prefix {magic}

%union {
    int v_int;
    char* v_str;
}

%initial-action
{
    magicline = 1;
    parsedElements.clear();
}

%token SPACE
%token MAGIC TECH MAGSCALE
%token TIMESTAMP
%token BEGINTITLE ENDTITLE

%token RECT
%token RLABEL

%token ENDL
%token END

%token INTEGER
%type <v_int> INTEGER
%token IDENT
%type <v_str> IDENT

%%

program:
    MAGIC endls
    TECH SPACE IDENT endls
    MAGSCALE SPACE INTEGER SPACE INTEGER endls
    TIMESTAMP SPACE INTEGER endls
    sections
    ;

sections:
    sections endls section
    |
    section
    ;

section:
    BEGINTITLE sectiontitle ENDTITLE endls items
    |
    END
    |
    %empty
    ;

sectiontitle:
    IDENT { title = QString($1); }
    ;

items:
    items endls item
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
    RECT SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER { parsedElements << Rectangle { $3, $5, $7 - $3, $9 - $5, title }; }
    ;

rlabel:
    RLABEL SPACE material
    SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE INTEGER SPACE
    IDENT
    ;

material:
    IDENT
    ;

endls:
    endls endl
    |
    endl
    ;

endl:
    ENDL    { ++magicline; }
    ;

%%

void yyerror(const char *s) {
    yyclearin;
    throw ParserException{magicline, QString(s)};
}

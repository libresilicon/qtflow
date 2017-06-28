%{
#include "common.h"

#include <iostream>
#include <string>

#include <QString>
#include <QMap>

int yylex();
void yyerror(const char *s);

int settingsline;
map_string_t parsedVariables;
QString settingskey;
QString settingsvalue;

%}

%defines "yysettings.h"
%output "yysettings.cpp"
%error-verbose

%define api.prefix {settings}

%union {
    char* v_str;
}

%initial-action
{
    settingsline = 1;
    parsedVariables.clear();
}

%token SET
%token ASSIGN
%token SPACES
%token ENDL
%token END

%token STRING
%type <v_str> STRING
%%

program:
    variables
    ;

variables:
    variables variable endls
    |
    variable
    |
    endls
    |
    %empty

variable:
    SET SPACES key spaces ASSIGN spaces value { parsedVariables.insert(settingskey, settingsvalue); }
    ;

spaces:
    SPACES spaces
    |
    SPACES
    |
    %empty
    ;

key:
    STRING { settingskey = QString($1); }
    ;

value:
    STRING { settingsvalue = QString($1); }
    ;

endls:
    endls endl
    |
    endl
    ;

endl:
    ENDL    { ++settingsline; }
    ;

%%

void yyerror(const char *s) {
    yyclearin;
    throw ParserException{settingsline, QString(s)};
}

%{
#include "common.h"

#include <iostream>
#include <string>

#include <QString>
#include <QMap>
#include <QStack>

int yylex();
void yyerror(const char *s);

int settingsline;
map_string_t parsedVariables;
QStack<QChar> settingskey;
QStack<QChar> settingsvalue;

%}

%defines "yysettings.h"
%output "yysettings.cpp"
%error-verbose

%define api.prefix {settings}

%union {
    char* v_str;
    char v_char;
}

%initial-action
{
    settingsline = 1;
    parsedVariables.clear();
}

%token SET
%token ASSIGN
%token SPACE
%token ENDL
%token END

%token ALPHANUM SPECIAL
%type <v_char> anychar ALPHANUM SPECIAL SPACE

%%

program:
    variables
    ;

variables:
    variables variable endls
    |
    variable endls
    |
    endls
    ;

variable:
    SET spaces key spaces ASSIGN spaces value
    {
        QString k;
        QString v;
        while (!settingskey.isEmpty())
            k.prepend(settingskey.pop());
        while (!settingsvalue.isEmpty())
            v.prepend(settingsvalue.pop());
        parsedVariables.insert(k, v);
    }
    ;

spaces:
    spaces SPACE
    |
    SPACE
    |
    %empty
    ;

key:
    key ALPHANUM { settingskey.push(QChar($2)); }
    |
    ALPHANUM { settingskey.push(QChar($1)); }
    |
    %empty
    ;

value:
    value anychar { settingsvalue.push(QChar($2)); }
    |
    anychar { settingsvalue.push(QChar($1)); }
    |
    %empty
    ;

anychar: ALPHANUM | SPECIAL | SPACE;

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

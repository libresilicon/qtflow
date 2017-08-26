%{
#include "common.h"

#include <iostream>
#include <string>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QStack>
#include <QDebug>

int yylex();
void yyerror(const char *s);

int vcdline;
vcd_t parsedVcd;
int timeIndex;
QStack<vcd_scopes_t*> parsedScopes;
QStack<QChar> vcdstring;

%}

%defines "yyvcd.h"
%output "yyvcd.cpp"
%error-verbose

%define api.prefix {vcd}

%union {
    char* v_str;
    char v_char;
}

%initial-action
{
    vcdline = 1;
    vcdstring.clear();
    parsedScopes.clear();
    parsedVcd.changes.clear();
}

%token ENDL
%token SPACE

%token END
%token DATE
%token VERSION
%token TIMESCALE
%token SCOPE UPSCOPE
%token VAR
%token ENDDEFINITIONS
%token DUMPVARS

%token POINT

%token ALPHANUM SPECIAL
%type <v_char> character ALPHANUM SPECIAL


%%

program:
    definitions
    ;

definitions:
    definitions definition endls
    |
    definition endls
    |
    endls
    ;

definition:
    date
    |
    version
    |
    timescale
    |
    scope | upscope
    |
    var
    |
    ENDDEFINITIONS spaces END
    |
    index
    |
    value
    |
    DUMPVARS | END
    ;

date:
    DATE endls string endls END
    {
        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());
        parsedVcd.date = v.trimmed();
    }
    ;

version:
    VERSION endls string endls END
    {
        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());
        parsedVcd.version = v.trimmed();
    }
    ;

timescale:
    TIMESCALE endls string endls END
    {
        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());
        QString time = v.trimmed();
        int pos = time.indexOf('s');
        if (pos > 0)
        {
            time.truncate(pos);
            parsedVcd.timescale = time.toInt() * 1000000;
        }
    }
    ;

scope:
    SCOPE string END
    {
        vcd_scopes_t s;
        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());
        s.name = v.trimmed().split(' ').at(1);
        if (parsedScopes.empty())
        {
            parsedVcd.scopes = s;
            parsedScopes.push(&(parsedVcd.scopes));
        }
        else
        {
            vcd_scopes_t *t = parsedScopes.pop();
            parsedScopes.push(t);
            t->scopes.append(s);
            parsedScopes.push(&(t->scopes.last()));
        }
    }
    ;

upscope:
    UPSCOPE spaces END
    {
        parsedScopes.pop();
    }
    ;

var:
    VAR string END
    {
        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());
        QStringList var = v.trimmed().split(' ');
        vcd_scopes_t *s = parsedScopes.pop();
        parsedScopes.push(s);
        s->alias.insert(var.at(2).at(0).unicode(), var.at(3));
    }
    ;

index:
    SPECIAL string
    {
        QString time;
        while (!vcdstring.isEmpty())
            time.prepend(vcdstring.pop());
        timeIndex = time.toInt();
    }
    ;

value:
    word spaces SPECIAL
    {
        int key = QChar($3).unicode();
        bool ok;
        int value = 0;

        QString v;
        while (!vcdstring.isEmpty())
            v.prepend(vcdstring.pop());

        if (v.at(0) == 'b')
            value = v.trimmed().right(v.size() - 1).toInt(&ok, 2);
        else
            value = v.toInt(&ok, 10);

        if (parsedVcd.changes.contains(key))
            parsedVcd.changes[key] << QPair<int, int>(timeIndex, value);
        else
            parsedVcd.changes.insert(key, vcd_changes_t() << QPair<int, int>(timeIndex, value));
    }
    ;

word:
    word ALPHANUM { vcdstring.push(QChar($2)); }
    |
    ALPHANUM { vcdstring.push(QChar($1)); }
    |
    %empty
    ;

string:
    string character { vcdstring.push(QChar($2)); }
    |
    character { vcdstring.push(QChar($1)); }
    |
    %empty
    ;

character: ALPHANUM | SPECIAL | SPACE;

spaces:
    spaces SPACE
    |
    SPACE
    |
    endls
    |
    %empty
    ;

endls:
    endls endl
    |
    endl
    ;

endl:
    ENDL    { ++vcdline; }
    ;


%%

void yyerror(const char *s) {
    yyclearin;
    throw ParserException{vcdline, QString(s)};
}

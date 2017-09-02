%defines "vcd.yy.h"
%language "c++"
%glr-parser
%debug
%error-verbose
%name-prefix="vcd"
%define "parser_class_name" {VCDParser}
%token-table

%code requires {
	namespace vcd {
		class VCData;
	}
}
%param {vcd::VCData *vcdata}

%{
#include <iostream>
#include <string>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QStack>
#include <QDebug>

#include "vcdata.h"

#define vcdlex (vcdata->getLexer())->vcdlex
#define vcdlineno (int)(vcdata->getLexer())->lineno()
%}

%union {
	std::string *v_str;
	char v_char;
	int v_int;
}

%token END
%token DATE
%token VERSION
%token TIMESCALE
%token SCOPE UPSCOPE
%token VAR
%token ENDDEFINITIONS
%token DUMPVARS
%token REGISTER
%token WIRE

%token POINT

%token <v_char> ALPHANUM
%token <v_char> SPECIAL
%token <v_char> character
%token <v_str> STRING
%token <v_int> INTEGER

%start vcd_file
%%

vcd_file:
	  vcd_file definition
	| definition
	;

definition:
	  date
	| version
	| timescale
	| scope
	| upscope
	| var
	| ENDDEFINITIONS END
	| value
	| DUMPVARS
	| END
	;

date: DATE STRING END
{
	vcdata->setDate($2);
}
;

version: VERSION STRING END
{
	vcdata->setVersion($2);
}
;

timescale: TIMESCALE STRING END
{
	vcdata->addTimescale($2);
};

scope: SCOPE STRING END
{
	vcdata->addScope($2);
};

upscope: UPSCOPE END;

reg: REGISTER INTEGER STRING STRING;

wire: WIRE INTEGER STRING STRING;

var:
	  VAR wire END
	| VAR reg END;

value: word SPECIAL;

word:
	  word ALPHANUM
	| ALPHANUM
	| %empty
;

%%

void vcd::VCDParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << vcdlineno << std::endl;
}

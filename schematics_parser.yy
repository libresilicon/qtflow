%defines "schematics.yy.h"
%error-verbose
%define "parser_class_name" {SchematicsParser}
%define api.prefix {schematics}
%language "c++"

%code requires {
	namespace schematics {
		class SchematicsData;
	}
}
%param {schematics::SchematicsData *schematicsdata}

%{
#include <iostream>
#include <string>

#include <QString>
#include <QMap>
#include <QStack>

#include "schematicsdata.h"

void schematics_error(const char *s);
#define schematicslex (schematicsdata->getLexer())->schematicslex
%}

%union {
	std::string* v_str;
	char v_char;
	int v_int;
}

%token LIBS
%token COMPONENT
%token END_COMPONENT

%%
component:
        COMPONENT component_content END_COMPONENT
;

component_content:
;

%%

void schematics_error(const char *s) {
        //yyclearin;
	//throw ParserException{settingsline, QString(s)};
}

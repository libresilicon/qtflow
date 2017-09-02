%define api.prefix {def}
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

%param {def::DEFData *defdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "def/defscanner.h"
#include "def/defdata.h"

#define deflex (defdata->getLexer())->deflex
#define deflineno (int)(defdata->getLexer())->lineno()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token VERSION
%token NAMESCASESENSITIVE
%token END
%token DESIGN
%token BUSBITCHARS
%token DISTANCE
%token DIVIDERCHAR
%token UNITS
%token MICRONS
%token BRACKETOPEN
%token BRACKETCLOSE
%token DIEAREA
%token COMPONENTS
%token TRACKS
%token DO
%token LAYER
%token STEP
%token MINUS
%token PLUS
%token PLACED
%token PINS
%token NET
%token NETS
%token ROUTED
%token ASTERISK
%token NEW

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%start def_file

%%

def_file: def_file_options END DESIGN;
def_file_options: def_file_option | def_file_options def_file_option;
def_file_option:
	  version
	| cases
	| bitchars
	| dividechar
	| units
	| design
	| diearea
	| tracks
	| components
	| pins
	| nets
	;

design: DESIGN STRING;
version: VERSION DOUBLE;
cases: NAMESCASESENSITIVE STRING;
bitchars: BUSBITCHARS STRING;
dividechar: DIVIDERCHAR STRING;
units: UNITS DISTANCE MICRONS INTEGER
	{
	defdata->setDistanceUnitMicrons($4);
	}
	;
diearea: DIEAREA BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE
	{
	}
	;

tracks: TRACKS STRING DOUBLE DO INTEGER STEP DOUBLE LAYER STRING | TRACKS STRING DOUBLE DO INTEGER STEP INTEGER LAYER STRING;
components: components_amount component_list END COMPONENTS;
components_amount: COMPONENTS INTEGER
	{
		defdata->setAmountComponents($2);
	}
	;
component_list:
	  component_list_element
	| component_list_element component_list
	;

component_list_element: MINUS STRING STRING PLUS PLACED BRACKETOPEN DOUBLE INTEGER BRACKETCLOSE STRING;

pins: PINS INTEGER pin_list END PINS;
pin_list:
	  pin_list_element
	| pin_list_element pin_list
	;
pin_list_element: MINUS STRING PLUS NET STRING pin_settings;
pin_settings:
	  PLUS pin_setting
	| PLUS pin_setting pin_settings
	;
pin_setting:
	  pin_setting_layer
	| pin_setting_position
	;

pin_setting_layer: LAYER STRING BRACKETOPEN INTEGER INTEGER BRACKETCLOSE BRACKETOPEN INTEGER INTEGER BRACKETCLOSE;
pin_setting_position: PLACED BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE STRING;

nets: NETS INTEGER net_list END NETS;
net_list:
	  net_list_element
	| net_list_element net_list
	;

net_list_element: MINUS STRING net_connections routed_info;

net_connections:
	  net_connection
	| net_connection net_connections
	;

net_connection: BRACKETOPEN STRING STRING BRACKETCLOSE;

routed_info:
	  routed_info_layer routed_info_tupels new_metal_list
	| routed_info_layer routed_info_tupels STRING new_metal_list;

routed_info_layer: PLUS ROUTED STRING;

routed_info_tupels:
	  routed_info_tupel
	| routed_info_tupel routed_info_tupels
	;

routed_info_tupel:
	  BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
	| BRACKETOPEN ASTERISK INTEGER BRACKETCLOSE
	| BRACKETOPEN INTEGER ASTERISK BRACKETCLOSE
	| BRACKETOPEN ASTERISK ASTERISK BRACKETCLOSE;

new_metal_tupels:
	  new_metal_tupel
	| new_metal_tupel new_metal_tupels
	;

new_metal_tupel:
	  BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
	| BRACKETOPEN ASTERISK INTEGER BRACKETCLOSE
	| BRACKETOPEN INTEGER ASTERISK BRACKETCLOSE
	| BRACKETOPEN ASTERISK ASTERISK BRACKETCLOSE;

new_metal_list:
	  new_metal
	| new_metal new_metal_list
	;

new_metal:
	  NEW STRING new_metal_tupels STRING
	| NEW STRING new_metal_tupels;

%%

void def::DEFParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << deflineno << std::endl;
}
